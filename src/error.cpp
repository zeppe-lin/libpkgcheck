// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later
#include <libpkgcheck/error.h>

#include <utility>

namespace pkgcheck {

std::string_view to_string(error_code value) noexcept
{
  switch (value) {
  case error_code::invalid_identity: return "invalid-identity";
  case error_code::identity_failed: return "identity-failed";
  case error_code::invalid_request: return "invalid-request";
  case error_code::wrong_action: return "wrong-action";
  case error_code::missing_build_predecessor: return "missing-build-predecessor";
  case error_code::inconsistent_transaction: return "inconsistent-transaction";
  case error_code::build_failed: return "build-failed";
  case error_code::inconsistent_build_authority: return "inconsistent-build-authority";
  case error_code::invalid_result: return "invalid-result";
  }
  return "unknown";
}

error::error(error_code code, std::string message)
    : std::runtime_error(std::move(message)), code_(code)
{
}

error_code error::code() const noexcept { return code_; }

} // namespace pkgcheck
