// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/check.h"
#include "../support/test.h"

#include <libpkgcheck/libpkgcheck.h>

int main()
{
  auto value = check_fixture::make_scenario();
  const auto& check = check_fixture::node(
      value.transaction, pkgtransaction::transaction_action_kind::check);

  const auto build = check_fixture::successful_build(value.transaction, 'b', 2);
  const auto repeated = pkgcheck::check_request::seal(
      value.transaction, check.identity(), build);
  const auto same = pkgcheck::check_request::seal(
      value.transaction, check.identity(), build);
  TEST_CHECK(repeated == same);
  TEST_CHECK(repeated.identity() == same.identity());

  const auto different_artifact = check_fixture::successful_build(
      value.transaction, 'd', 2);
  const auto artifact_request = pkgcheck::check_request::seal(
      value.transaction, check.identity(), different_artifact);
  TEST_CHECK(artifact_request.identity() != repeated.identity());
  TEST_CHECK(artifact_request.inputs() == repeated.inputs());

  const auto different_policy = check_fixture::successful_build(
      value.transaction, 'b', 8);
  const auto policy_request = pkgcheck::check_request::seal(
      value.transaction, check.identity(), different_policy);
  TEST_CHECK(policy_request.identity() != repeated.identity());
  TEST_CHECK(policy_request.inputs() == repeated.inputs());
  TEST_CHECK(policy_request.program() == repeated.program());
  return 0;
}
