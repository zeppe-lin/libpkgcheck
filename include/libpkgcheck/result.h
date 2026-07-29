// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

/*! \file result.h
 *  \brief Terminal check outcomes bound to exact execution evidence.
 */
#pragma once

#include <optional>

#include <libpkgcheck/request.h>

namespace pkgcheck {

/*! \brief Complete terminal evidence for one sealed check request. */
class check_result final {
public:
  [[nodiscard]] static check_result passed(
      check_request request,
      check_execution_evidence_identity execution);

  [[nodiscard]] static check_result failed(
      check_request request,
      check_execution_evidence_identity execution,
      check_failure_kind failure,
      check_failure_evidence_identity failure_evidence);

  [[nodiscard]] check_outcome outcome() const noexcept;
  [[nodiscard]] const check_request& request() const noexcept;
  [[nodiscard]] const check_execution_evidence_identity&
  execution_evidence() const noexcept;
  [[nodiscard]] const std::optional<check_failure_kind>&
  failure() const noexcept;
  [[nodiscard]] const std::optional<check_failure_evidence_identity>&
  failure_evidence() const noexcept;
  [[nodiscard]] const check_result_identity& identity() const noexcept;

  friend bool operator==(const check_result& lhs,
                         const check_result& rhs) noexcept;
  friend bool operator!=(const check_result& lhs,
                         const check_result& rhs) noexcept;
private:
  check_result(
      check_outcome outcome,
      check_request request,
      check_execution_evidence_identity execution,
      std::optional<check_failure_kind> failure,
      std::optional<check_failure_evidence_identity> failure_evidence,
      check_result_identity identity);

  check_outcome outcome_;
  check_request request_;
  check_execution_evidence_identity execution_;
  std::optional<check_failure_kind> failure_;
  std::optional<check_failure_evidence_identity> failure_evidence_;
  check_result_identity identity_;
};

} // namespace pkgcheck
