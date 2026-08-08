// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/check.h"
#include "../support/test.h"

#include <libpkgcheck/libpkgcheck.h>

#include <array>
#include <string>

int main()
{
  using namespace pkgcheck;

  const auto request = check_fixture::admitted_request();
  const auto execution = check_execution_evidence_identity::from_sha256(
      std::string(64, 'e'));
  const auto other_execution = check_execution_evidence_identity::from_sha256(
      std::string(64, 'd'));
  const auto failure = check_failure_evidence_identity::from_sha256(
      std::string(64, 'f'));
  const auto other_failure = check_failure_evidence_identity::from_sha256(
      std::string(64, 'c'));

  const auto passed = check_result::passed(request, execution);
  const auto repeated = check_result::passed(request, execution);
  TEST_CHECK(passed == repeated);
  TEST_CHECK(passed.outcome() == check_outcome::passed);
  TEST_CHECK(!passed.failure());
  TEST_CHECK(!passed.failure_evidence());
  TEST_CHECK(passed.execution_evidence() == execution);
  TEST_CHECK(passed.request().identity() == request.identity());
  TEST_CHECK(check_result::passed(request, other_execution).identity() !=
             passed.identity());

  std::array<check_result, 3> failed{{
      check_result::failed(
          request, execution, check_failure_kind::execution_unavailable,
          failure),
      check_result::failed(
          request, execution, check_failure_kind::program_failed, failure),
      check_result::failed(
          request, execution, check_failure_kind::cancelled, failure),
  }};
  for (const auto& value : failed) {
    TEST_CHECK(value.outcome() == check_outcome::failed);
    TEST_CHECK(value.failure());
    TEST_CHECK(value.failure_evidence());
    TEST_CHECK(*value.failure_evidence() == failure);
    TEST_CHECK(value.identity() != passed.identity());
  }
  TEST_CHECK(failed[0].identity() != failed[1].identity());
  TEST_CHECK(failed[1].identity() != failed[2].identity());
  TEST_CHECK(check_result::failed(
                 request, execution, check_failure_kind::program_failed,
                 other_failure).identity() != failed[1].identity());
  TEST_CHECK(check_result::failed(
                 request, other_execution, check_failure_kind::program_failed,
                 failure).identity() != failed[1].identity());
  return 0;
}
