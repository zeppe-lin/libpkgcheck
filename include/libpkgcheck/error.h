// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

/*! \file error.h
 *  \brief Check-authority admission and result failures.
 */
#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

namespace pkgcheck {

enum class error_code {
  invalid_identity,
  identity_failed,
  invalid_request,
  wrong_action,
  missing_build_predecessor,
  inconsistent_transaction,
  build_failed,
  inconsistent_build_authority,
  invalid_result,
};

[[nodiscard]] std::string_view to_string(error_code value) noexcept;

class error final : public std::runtime_error {
public:
  error(error_code code, std::string message);
  [[nodiscard]] error_code code() const noexcept;
private:
  error_code code_;
};

} // namespace pkgcheck
