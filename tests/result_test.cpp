// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later
#include "check_fixture.h"
#include "test.h"

#include <libpkgcheck/libpkgcheck.h>

#include <string>

int main()
{
  using namespace pkgcheck;

  const auto request = check_fixture::admitted_request();
  const auto execution = check_execution_evidence_identity::from_sha256(
      std::string(64, 'e'));
  const auto failure = check_failure_evidence_identity::from_sha256(
      std::string(64, 'f'));

  const auto passed = check_result::passed(request, execution);
  const auto repeated = check_result::passed(request, execution);
  TEST_CHECK(passed.outcome() == check_outcome::passed);
  TEST_CHECK(!passed.failure());
  TEST_CHECK(!passed.failure_evidence());
  TEST_CHECK(passed.execution_evidence() == execution);
  TEST_CHECK(passed.request().identity() == request.identity());
  TEST_CHECK(passed.identity() == repeated.identity());

  const auto failed = check_result::failed(
      request, execution, check_failure_kind::program_failed, failure);
  TEST_CHECK(failed.outcome() == check_outcome::failed);
  TEST_CHECK(failed.failure() &&
             *failed.failure() == check_failure_kind::program_failed);
  TEST_CHECK(failed.failure_evidence() &&
             *failed.failure_evidence() == failure);
  TEST_CHECK(failed.identity() != passed.identity());

  const auto cancelled = check_result::failed(
      request, execution, check_failure_kind::cancelled, failure);
  TEST_CHECK(cancelled.identity() != failed.identity());
  TEST_CHECK(to_string(check_outcome::passed) == "passed");
  TEST_CHECK(to_string(check_failure_kind::execution_unavailable) ==
             "execution-unavailable");

  bool invalid_identity = false;
  try {
    (void)check_result_identity::from_sha256("invalid");
  } catch (const error& problem) {
    invalid_identity = problem.code() == error_code::invalid_identity;
  }
  TEST_CHECK(invalid_identity);

  return 0;
}
