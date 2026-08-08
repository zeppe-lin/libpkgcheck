// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../support/test.h"

#include <libpkgcheck/libpkgcheck.h>

#include <array>
#include <string_view>

int main()
{
  using namespace pkgcheck;

  TEST_CHECK(to_string(check_outcome::passed) == "passed");
  TEST_CHECK(to_string(check_outcome::failed) == "failed");
  TEST_CHECK(to_string(static_cast<check_outcome>(99)) == "unknown");

  TEST_CHECK(to_string(check_failure_kind::execution_unavailable) ==
             "execution-unavailable");
  TEST_CHECK(to_string(check_failure_kind::program_failed) ==
             "program-failed");
  TEST_CHECK(to_string(check_failure_kind::cancelled) == "cancelled");
  TEST_CHECK(to_string(static_cast<check_failure_kind>(99)) == "unknown");

  const std::array<std::pair<error_code, std::string_view>, 9> errors{{
      {error_code::invalid_identity, "invalid-identity"},
      {error_code::identity_failed, "identity-failed"},
      {error_code::invalid_request, "invalid-request"},
      {error_code::wrong_action, "wrong-action"},
      {error_code::missing_build_predecessor, "missing-build-predecessor"},
      {error_code::inconsistent_transaction, "inconsistent-transaction"},
      {error_code::build_failed, "build-failed"},
      {error_code::inconsistent_build_authority,
       "inconsistent-build-authority"},
      {error_code::invalid_result, "invalid-result"},
  }};
  for (const auto& [code, text] : errors)
    TEST_CHECK(to_string(code) == text);
  TEST_CHECK(to_string(static_cast<error_code>(99)) == "unknown");

  error problem(error_code::invalid_request, "bad request");
  TEST_CHECK(problem.code() == error_code::invalid_request);
  TEST_CHECK(std::string_view(problem.what()) == "bad request");
  return 0;
}
