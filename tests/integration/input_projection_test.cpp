// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../fixtures/check.h"
#include "../support/test.h"

#include <libpkgcheck/libpkgcheck.h>

#include <algorithm>

int main()
{
  auto value = check_fixture::make_multi_input_scenario();
  const auto build = check_fixture::request(value.transaction);
  const auto expected = build.inputs().for_scope(pkgbuild::input_scope::check);
  const auto projected = pkgcheck::check_input_set::project(build);
  const auto repeated = pkgcheck::check_input_set::project(build);

  TEST_CHECK(projected == repeated);
  TEST_CHECK(projected.identity() == repeated.identity());
  TEST_CHECK(projected.inputs() == expected);
  TEST_CHECK(projected.inputs().size() == 2U);
  TEST_CHECK(std::is_sorted(projected.inputs().begin(), projected.inputs().end()));
  for (const auto& input : projected.inputs()) {
    TEST_CHECK(input.scope() == pkgbuild::input_scope::check);
    TEST_CHECK(input.package().name() != "builder");
  }

  auto simple = check_fixture::make_scenario();
  const auto simple_projected = pkgcheck::check_input_set::project(
      check_fixture::request(simple.transaction));
  TEST_CHECK(simple_projected.inputs().size() == 1U);
  TEST_CHECK(simple_projected.identity() != projected.identity());
  return 0;
}
