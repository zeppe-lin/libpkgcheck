// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/check.h"
#include "../support/test.h"

#include <libpkgcheck/libpkgcheck.h>

int main()
{
  using namespace pkgcheck;

  auto value = check_fixture::make_scenario();
  const auto& check = check_fixture::node(
      value.transaction, pkgtransaction::transaction_action_kind::check);
  const auto& build_node = check_fixture::node(
      value.transaction, pkgtransaction::transaction_action_kind::build);
  const auto build = check_fixture::successful_build(value.transaction);
  const auto request = check_request::seal(
      value.transaction, check.identity(), build);

  TEST_CHECK(request.transaction() == value.transaction.identity());
  TEST_CHECK(request.build_node().identity() == build_node.identity());
  TEST_CHECK(request.check_node().identity() == check.identity());
  TEST_CHECK(request.build().identity() == build.identity());
  TEST_CHECK(request.program().language() ==
             pkgsource::program_language::posix_shell);
  TEST_CHECK(request.program().material() == "printf 'checked\\n'\n");
  TEST_CHECK(request.inputs().inputs().size() == 1U);
  TEST_CHECK(request.inputs().inputs().front().package().name() == "tester");

  TEST_PKGCHECK_THROWS(
      error_code::wrong_action,
      check_request::seal(value.transaction, build_node.identity(), build));
  TEST_PKGCHECK_THROWS(
      error_code::build_failed,
      check_request::seal(
          value.transaction, check.identity(),
          check_fixture::failed_build(value.transaction)));

  auto foreign = check_fixture::make_scenario("true\n");
  const auto& foreign_check = check_fixture::node(
      foreign.transaction, pkgtransaction::transaction_action_kind::check);
  TEST_PKGCHECK_THROWS(
      error_code::inconsistent_transaction,
      check_request::seal(value.transaction, foreign_check.identity(), build));
  return 0;
}
