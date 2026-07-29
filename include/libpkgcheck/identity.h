// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

/*! \file identity.h
 *  \brief Domain-separated SHA-256 check identities.
 */
#pragma once

#include <string>

namespace pkgcheck {

#define PKGCHECK_DECLARE_IDENTITY(type_name)                                   \
class type_name final {                                                        \
public:                                                                        \
  [[nodiscard]] static type_name from_sha256(std::string hex);                 \
  [[nodiscard]] const std::string& hex() const noexcept;                       \
  friend bool operator==(const type_name& lhs, const type_name& rhs) noexcept; \
  friend bool operator!=(const type_name& lhs, const type_name& rhs) noexcept; \
  friend bool operator<(const type_name& lhs, const type_name& rhs) noexcept;  \
private:                                                                       \
  explicit type_name(std::string hex);                                         \
  std::string hex_;                                                            \
}

PKGCHECK_DECLARE_IDENTITY(check_input_set_identity);
PKGCHECK_DECLARE_IDENTITY(check_request_identity);
PKGCHECK_DECLARE_IDENTITY(check_execution_evidence_identity);
PKGCHECK_DECLARE_IDENTITY(check_failure_evidence_identity);
PKGCHECK_DECLARE_IDENTITY(check_result_identity);

#undef PKGCHECK_DECLARE_IDENTITY

} // namespace pkgcheck
