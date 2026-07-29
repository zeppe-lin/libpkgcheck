// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later
#include <libpkgcheck/result.h>

#include <libpkgcheck/error.h>

#include "identity_support.h"

#include <utility>

namespace pkgcheck {
namespace {

check_result_identity identify_passed(
    const check_request& request,
    const check_execution_evidence_identity& execution)
{
  detail::identity_writer writer;
  writer.text("pkgcheck/check-result/passed/v1");
  writer.text(request.identity().hex());
  writer.text(execution.hex());
  return check_result_identity::from_sha256(writer.finish());
}

check_result_identity identify_failed(
    const check_request& request,
    const check_execution_evidence_identity& execution,
    check_failure_kind failure,
    const check_failure_evidence_identity& failure_evidence)
{
  detail::identity_writer writer;
  writer.text("pkgcheck/check-result/failed/v1");
  writer.text(request.identity().hex());
  writer.text(execution.hex());
  writer.text(to_string(failure));
  writer.text(failure_evidence.hex());
  return check_result_identity::from_sha256(writer.finish());
}

} // namespace

check_result::check_result(
    check_outcome outcome,
    check_request request,
    check_execution_evidence_identity execution,
    std::optional<check_failure_kind> failure,
    std::optional<check_failure_evidence_identity> failure_evidence,
    check_result_identity identity)
    : outcome_(outcome), request_(std::move(request)),
      execution_(std::move(execution)), failure_(failure),
      failure_evidence_(std::move(failure_evidence)),
      identity_(std::move(identity))
{
  if ((outcome_ == check_outcome::failed) != failure_.has_value() ||
      failure_.has_value() != failure_evidence_.has_value())
    throw error(error_code::invalid_result,
                "check result failure fields do not match its outcome");
}

check_result check_result::passed(
    check_request request,
    check_execution_evidence_identity execution)
{
  auto identity = identify_passed(request, execution);
  return check_result(check_outcome::passed, std::move(request),
                      std::move(execution), std::nullopt, std::nullopt,
                      std::move(identity));
}

check_result check_result::failed(
    check_request request,
    check_execution_evidence_identity execution,
    check_failure_kind failure,
    check_failure_evidence_identity failure_evidence)
{
  auto identity = identify_failed(request, execution, failure,
                                  failure_evidence);
  return check_result(check_outcome::failed, std::move(request),
                      std::move(execution), failure,
                      std::move(failure_evidence), std::move(identity));
}

check_outcome check_result::outcome() const noexcept { return outcome_; }
const check_request& check_result::request() const noexcept { return request_; }
const check_execution_evidence_identity&
check_result::execution_evidence() const noexcept { return execution_; }
const std::optional<check_failure_kind>&
check_result::failure() const noexcept { return failure_; }
const std::optional<check_failure_evidence_identity>&
check_result::failure_evidence() const noexcept { return failure_evidence_; }
const check_result_identity& check_result::identity() const noexcept
{ return identity_; }

bool operator==(const check_result& lhs,
                const check_result& rhs) noexcept
{
  return lhs.identity_ == rhs.identity_;
}

bool operator!=(const check_result& lhs,
                const check_result& rhs) noexcept
{
  return !(lhs == rhs);
}

} // namespace pkgcheck
