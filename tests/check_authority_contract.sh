#!/bin/sh
# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
set -eu
root=$1
request=$root/src/request.cpp
result=$root/src/result.cpp

grep -q 'transaction_action_kind::check' "$request"
grep -q 'phase_order_kind::build_before_check' "$request"
grep -q 'build_outcome::succeeded' "$request"
grep -q 'recipe().check_program()' "$request"
grep -q 'input_scope::check' "$root/src/model.cpp"
grep -q 'pkgbuild::build_input' "$root/include/libpkgcheck/model.h"
! grep -R -q 'materialized_package_input\|input_tree_identity' "$root/include" "$root/src"
grep -q 'pkgcheck/check-request/v1' "$request"
grep -q 'pkgcheck/check-result/passed/v1' "$result"
grep -q 'pkgcheck/check-result/failed/v1' "$result"
! grep -R -q 'execution_backend' "$root/include" "$root/src"
! grep -R -q 'std::filesystem' "$root/include" "$root/src"
