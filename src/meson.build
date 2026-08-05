# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
public_inc = include_directories('../include')
private_inc = include_directories('.')

libpkgcheck = library(
  'pkgcheck',
  [
    'error.cpp',
    'identity.cpp',
    'identity_support.cpp',
    'model.cpp',
    'request.cpp',
    'result.cpp',
  ],
  include_directories: [public_inc, private_inc],
  dependencies: [libcrypto_dep, libpkgbuild_dep, libpkgtransaction_dep],
  install: true,
  version: meson.project_version(),
  soversion: '0',
)

libpkgcheck_dep = declare_dependency(
  link_with: libpkgcheck,
  include_directories: public_inc,
  dependencies: [libcrypto_dep, libpkgbuild_dep, libpkgtransaction_dep],
)
meson.override_dependency('libpkgcheck', libpkgcheck_dep)

install_headers(
  '../include/libpkgcheck/libpkgcheck.h',
  '../include/libpkgcheck/error.h',
  '../include/libpkgcheck/identity.h',
  '../include/libpkgcheck/model.h',
  '../include/libpkgcheck/request.h',
  '../include/libpkgcheck/result.h',
  subdir: 'libpkgcheck',
)

pkgconf.generate(
  libpkgcheck,
  name: 'libpkgcheck',
  filebase: 'libpkgcheck',
  description: 'Transaction-bound package check authority',
  subdirs: '.',
  requires: [
    'libpkgbuild >= 3.0.0',
    'libpkgbuild < 4.0.0',
    'libpkgtransaction >= 2.1.0',
    'libpkgtransaction < 3.0.0',
  ],
  requires_private: ['libcrypto'],
)
