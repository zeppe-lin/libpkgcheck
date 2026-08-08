// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../support/test.h"

#include <libpkgcheck/libpkgcheck.h>

#include <string>

template<typename Identity>
void check_identity()
{
  const auto first = Identity::from_sha256(std::string(64, 'a'));
  const auto repeated = Identity::from_sha256(std::string(64, 'a'));
  const auto second = Identity::from_sha256(std::string(64, 'b'));
  TEST_CHECK(first == repeated);
  TEST_CHECK(first != second);
  TEST_CHECK(first < second);
  TEST_CHECK(first.hex() == std::string(64, 'a'));

  TEST_PKGCHECK_THROWS(
      pkgcheck::error_code::invalid_identity,
      Identity::from_sha256(std::string(63, 'a')));
  TEST_PKGCHECK_THROWS(
      pkgcheck::error_code::invalid_identity,
      Identity::from_sha256(std::string(64, 'A')));
  TEST_PKGCHECK_THROWS(
      pkgcheck::error_code::invalid_identity,
      Identity::from_sha256(std::string(64, 'g')));
}

int main()
{
  check_identity<pkgcheck::check_input_set_identity>();
  check_identity<pkgcheck::check_request_identity>();
  check_identity<pkgcheck::check_execution_evidence_identity>();
  check_identity<pkgcheck::check_failure_evidence_identity>();
  check_identity<pkgcheck::check_result_identity>();
  return 0;
}
