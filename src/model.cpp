// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later
#include <libpkgcheck/model.h>

#include "identity_support.h"

#include <utility>

namespace pkgcheck {
namespace {

check_input_set_identity identify_inputs(
    const std::vector<pkgbuild::build_input>& inputs)
{
  detail::identity_writer writer;
  writer.text("pkgcheck/check-input-set/v1");
  writer.number(inputs.size());
  for (const auto& input : inputs) {
    writer.text(input.identity().hex());
  }
  return check_input_set_identity::from_sha256(writer.finish());
}

} // namespace

std::string_view to_string(check_outcome value) noexcept
{
  switch (value) {
  case check_outcome::passed: return "passed";
  case check_outcome::failed: return "failed";
  }
  return "unknown";
}

std::string_view to_string(check_failure_kind value) noexcept
{
  switch (value) {
  case check_failure_kind::execution_unavailable:
    return "execution-unavailable";
  case check_failure_kind::program_failed: return "program-failed";
  case check_failure_kind::cancelled: return "cancelled";
  }
  return "unknown";
}

check_input_set::check_input_set(
    std::vector<pkgbuild::build_input> inputs,
    check_input_set_identity identity)
    : inputs_(std::move(inputs)), identity_(std::move(identity))
{
}

check_input_set check_input_set::project(
    const pkgbuild::build_request& build)
{
  auto inputs = build.inputs().for_scope(pkgbuild::input_scope::check);
  return check_input_set(inputs, identify_inputs(inputs));
}

const std::vector<pkgbuild::build_input>&
check_input_set::inputs() const noexcept
{
  return inputs_;
}

const check_input_set_identity& check_input_set::identity() const noexcept
{
  return identity_;
}

bool operator==(const check_input_set& lhs,
                const check_input_set& rhs) noexcept
{
  return lhs.identity_ == rhs.identity_ && lhs.inputs_ == rhs.inputs_;
}

bool operator!=(const check_input_set& lhs,
                const check_input_set& rhs) noexcept
{
  return !(lhs == rhs);
}

} // namespace pkgcheck
