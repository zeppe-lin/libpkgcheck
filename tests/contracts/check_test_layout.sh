#!/bin/sh
# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
set -eu
root=${1:?}
meson=$root/tests/meson.build

for directory in contracts fixtures header integration support unit; do
  test -d "$root/tests/$directory" || {
    echo "missing test role directory: $directory" >&2
    exit 1
  }
done
for suite in unit integration header contract; do
  grep -F "suite: '$suite'" "$meson" >/dev/null || {
    echo "missing Meson test suite: $suite" >&2
    exit 1
  }
done
for stale in request_test.cpp result_test.cpp public_headers.cpp check_fixture.h fixture.h fixture_transaction.h test.h; do
  test ! -e "$root/tests/$stale" || {
    echo "flat test artifact remains: $stale" >&2
    exit 1
  }
done

grep -F "'integration/input_projection_test.cpp'" "$meson" >/dev/null
grep -F "'integration/request_admission_test.cpp'" "$meson" >/dev/null
grep -F "'integration/build_authority_test.cpp'" "$meson" >/dev/null
grep -F "'integration/request_identity_test.cpp'" "$meson" >/dev/null
grep -F "'integration/result_semantics_test.cpp'" "$meson" >/dev/null
grep -F "test('header-' + header.underscorify()" "$meson" >/dev/null
! grep -F "test('header:" "$meson" >/dev/null
