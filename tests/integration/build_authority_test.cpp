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

  auto changed_source = check_fixture::make_scenario("printf 'changed\\n'\n");
  TEST_PKGCHECK_THROWS(
      error_code::inconsistent_build_authority,
      check_request::seal(
          value.transaction, check.identity(),
          check_fixture::successful_build(changed_source.transaction)));

  auto foreign_resolution = check_fixture::make_scenario(
      "printf 'checked\\n'\n",
      pkgresolve::installed_preference::prefer_catalog);
  const auto& foreign_build_node = check_fixture::node(
      foreign_resolution.transaction,
      pkgtransaction::transaction_action_kind::build);
  const auto foreign_build =
      check_fixture::successful_build(foreign_resolution.transaction);

  TEST_CHECK(build_node.selection());
  TEST_CHECK(foreign_build_node.selection());
  TEST_CHECK(build_node.selection()->identity() !=
             foreign_build_node.selection()->identity());
  TEST_CHECK(build.request().release().identity() ==
             foreign_build.request().release().identity());
  TEST_CHECK(build.request().source().identity() ==
             foreign_build.request().source().identity());
  TEST_CHECK(build.request().architectures().build() ==
             foreign_build.request().architectures().build());
  TEST_CHECK(build.request().architectures().target() ==
             foreign_build.request().architectures().target());
  TEST_CHECK(build.request().subject().identity() !=
             foreign_build.request().subject().identity());
  TEST_CHECK(build.request().inputs().resolution() !=
             foreign_build.request().inputs().resolution());

  TEST_PKGCHECK_THROWS(
      error_code::inconsistent_build_authority,
      check_request::seal(value.transaction, check.identity(), foreign_build));
  return 0;
}
