# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
if get_option('tests').enabled()
  foreach item : [
    ['request', 'request_test.cpp'],
    ['result', 'result_test.cpp'],
    ['public-headers', 'public_headers.cpp'],
  ]
    executable_test = executable(
      item[0] + '-test',
      item[1],
      dependencies: libpkgcheck_dep,
      install: false,
    )
    test(item[0], executable_test)
  endforeach

  sh = find_program('sh')
  test(
    'authority-contract',
    sh,
    args: [join_paths(meson.current_source_dir(),
                      'check_authority_contract.sh'),
           meson.project_source_root()],
  )
  test(
    'release-metadata',
    sh,
    args: [join_paths(meson.current_source_dir(),
                      'check_release_metadata.sh'),
           meson.project_source_root()],
  )
endif
