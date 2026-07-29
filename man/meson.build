# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later
scdoc = find_program('scdoc', required: get_option('man_pages'))
if scdoc.found()
  foreach page : [
    ['libpkgcheck.3.scdoc', 'libpkgcheck.3'],
    ['pkgcheck_request.3.scdoc', 'pkgcheck_request.3'],
    ['pkgcheck_result.3.scdoc', 'pkgcheck_result.3'],
  ]
    custom_target(
      page[1],
      input: page[0],
      output: page[1],
      command: [scdoc],
      feed: true,
      capture: true,
      install: true,
      install_dir: join_paths(get_option('mandir'), 'man3'),
    )
  endforeach
endif
