#!/bin/sh
# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
set -eu
root=$1

grep -q "version: '0.2.0'" "$root/meson.build"
grep -q "soversion: '0'" "$root/src/meson.build"
grep -q "'libpkgbuild >= 3.0.0'" "$root/src/meson.build"
grep -q "'libpkgbuild < 4.0.0'" "$root/src/meson.build"
grep -q "'libpkgtransaction >= 2.1.0'" "$root/src/meson.build"
grep -q "'libpkgtransaction < 3.0.0'" "$root/src/meson.build"
grep -q '^libpkgcheck(3)$' "$root/man/libpkgcheck.3.scdoc"
grep -q '^0.2.0' "$root/HISTORY.md"
