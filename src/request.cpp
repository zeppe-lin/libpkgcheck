// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later
#include <libpkgcheck/request.h>

#include <libpkgcheck/error.h>

#include "identity_support.h"

#include <optional>
#include <utility>

namespace pkgcheck {
namespace {

const pkgtransaction::transaction_node& require_node(
    const pkgtransaction::transaction_program& transaction,
    const pkgtransaction::transaction_node_identity& identity)
{
  const auto* node = transaction.find(identity);
  if (!node)
    throw error(error_code::inconsistent_transaction,
                "transaction does not contain the requested check node");
  return *node;
}

const pkgtransaction::transaction_node& require_build_predecessor(
    const pkgtransaction::transaction_program& transaction,
    const pkgtransaction::transaction_node& check)
{
  const pkgtransaction::transaction_edge* witness = nullptr;
  for (const auto& edge : transaction.edges()) {
    if (edge.kind() != pkgtransaction::transaction_edge_kind::phase ||
        edge.after() != check.identity() || !edge.phase_order() ||
        *edge.phase_order() !=
            pkgtransaction::phase_order_kind::build_before_check)
      continue;
    if (witness)
      throw error(error_code::inconsistent_transaction,
                  "check node has more than one build predecessor");
    witness = &edge;
  }
  if (!witness)
    throw error(error_code::missing_build_predecessor,
                "check node has no build-before-check predecessor");
  const auto* build = transaction.find(witness->before());
  if (!build ||
      build->action() != pkgtransaction::transaction_action_kind::build)
    throw error(error_code::inconsistent_transaction,
                "check predecessor is not a transaction build node");
  return *build;
}

void require_same_selection(
    const pkgtransaction::transaction_node& build,
    const pkgtransaction::transaction_node& check)
{
  if (!build.selection() || !check.selection() ||
      build.selection()->identity() != check.selection()->identity())
    throw error(error_code::inconsistent_transaction,
                "build and check nodes do not retain the same selection");
}

void require_build_authority(
    const pkgtransaction::transaction_program& transaction,
    const pkgtransaction::transaction_node& build_node,
    const pkgtransaction::transaction_node& check_node,
    const pkgbuild::build_result& build)
{
  if (build.outcome() != pkgbuild::build_outcome::succeeded ||
      !build.payload() || !build.artifact() || !build.artifact_binding())
    throw error(error_code::build_failed,
                "check admission requires a successful complete build result");

  const auto* selection = check_node.selection();
  const auto* build_selection = build_node.selection();
  const auto* program = check_node.check_program();
  if (!selection || !build_selection || !program)
    throw error(error_code::inconsistent_transaction,
                "check transaction authority is incomplete");

  const auto& request = build.request();
  const auto& source = request.source();
  const auto& resolution = transaction.request().resolution();
  const auto& source_program = source.recipe().check_program();
  if (request.subject().identity() != selection->identity() ||
      request.inputs().resolution() != resolution.identity() ||
      request.release().identity() != selection->release().identity() ||
      source.identity() != selection->source_snapshot() ||
      request.architectures().build() != selection->architectures().build() ||
      request.architectures().target() != selection->architectures().target() ||
      !source_program || *source_program != *program ||
      build_selection->identity() != selection->identity())
    throw error(error_code::inconsistent_build_authority,
                "build result does not realize the check selection authority");
}

check_request_identity identify_request(
    const pkgtransaction::transaction_program_identity& transaction,
    const pkgtransaction::transaction_node& build_node,
    const pkgtransaction::transaction_node& check_node,
    const pkgbuild::build_result& build,
    const check_input_set& inputs)
{
  detail::identity_writer writer;
  writer.text("pkgcheck/check-request/v1");
  writer.text(transaction.hex());
  writer.text(build_node.identity().hex());
  writer.text(check_node.identity().hex());
  writer.text(build.identity().hex());
  writer.text(inputs.identity().hex());
  return check_request_identity::from_sha256(writer.finish());
}

} // namespace

check_request::check_request(
    pkgtransaction::transaction_program_identity transaction,
    pkgtransaction::transaction_node build_node,
    pkgtransaction::transaction_node check_node,
    pkgbuild::build_result build,
    check_input_set inputs,
    check_request_identity identity)
    : transaction_(std::move(transaction)), build_node_(std::move(build_node)),
      check_node_(std::move(check_node)), build_(std::move(build)),
      inputs_(std::move(inputs)), identity_(std::move(identity))
{
}

check_request check_request::seal(
    const pkgtransaction::transaction_program& transaction,
    const pkgtransaction::transaction_node_identity& check_node_identity,
    pkgbuild::build_result build)
{
  const auto& check_node = require_node(transaction, check_node_identity);
  if (check_node.action() != pkgtransaction::transaction_action_kind::check)
    throw error(error_code::wrong_action,
                "requested transaction node is not a check action");
  if (!check_node.check_program())
    throw error(error_code::invalid_request,
                "check node does not retain an exact source program");

  const auto& build_node = require_build_predecessor(transaction, check_node);
  require_same_selection(build_node, check_node);
  require_build_authority(transaction, build_node, check_node, build);
  auto inputs = check_input_set::project(build.request());
  auto identity = identify_request(transaction.identity(), build_node,
                                   check_node, build, inputs);
  return check_request(transaction.identity(), build_node, check_node,
                       std::move(build), std::move(inputs),
                       std::move(identity));
}

const pkgtransaction::transaction_program_identity&
check_request::transaction() const noexcept
{
  return transaction_;
}

const pkgtransaction::transaction_node&
check_request::build_node() const noexcept
{
  return build_node_;
}

const pkgtransaction::transaction_node&
check_request::check_node() const noexcept
{
  return check_node_;
}

const pkgbuild::build_result& check_request::build() const noexcept
{
  return build_;
}

const pkgsource::program& check_request::program() const noexcept
{
  return *check_node_.check_program();
}

const check_input_set& check_request::inputs() const noexcept
{
  return inputs_;
}

const check_request_identity& check_request::identity() const noexcept
{
  return identity_;
}

bool operator==(const check_request& lhs,
                const check_request& rhs) noexcept
{
  return lhs.identity_ == rhs.identity_;
}

bool operator!=(const check_request& lhs,
                const check_request& rhs) noexcept
{
  return !(lhs == rhs);
}

} // namespace pkgcheck
