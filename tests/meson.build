# SPDX-FileCopyrightText: 2026 Alexandr Savca
# SPDX-License-Identifier: GPL-3.0-or-later

if get_option('tests').allowed()
  unit_tests = {
    'identity': 'unit/identity_test.cpp',
    'model': 'unit/model_test.cpp',
  }

  integration_tests = {
    'input-projection': 'integration/input_projection_test.cpp',
    'request-admission': 'integration/request_admission_test.cpp',
    'build-authority': 'integration/build_authority_test.cpp',
    'request-identity': 'integration/request_identity_test.cpp',
    'result-semantics': 'integration/result_semantics_test.cpp',
  }

  foreach name, source : unit_tests
    executable_test = executable(
      name + '-test',
      source,
      dependencies: libpkgcheck_dep,
      install: false,
    )
    test(name, executable_test, suite: 'unit')
  endforeach

  foreach name, source : integration_tests
    executable_test = executable(
      name + '-test',
      source,
      dependencies: libpkgcheck_dep,
      install: false,
    )
    test(name, executable_test, suite: 'integration')
  endforeach

  public_headers = [
    'error.h',
    'identity.h',
    'model.h',
    'request.h',
    'result.h',
    'libpkgcheck.h',
  ]
  foreach header : public_headers
    header_test = executable(
      'header-' + header.underscorify() + '-test',
      'header/public_header_test.cpp',
      cpp_args: [
        '-DPKGCHECK_TEST_HEADER=<libpkgcheck/' + header + '>',
      ],
      dependencies: libpkgcheck_dep,
      install: false,
    )
    test('header-' + header.underscorify(), header_test, suite: 'header')
  endforeach

  contract_tests = {
    'authority-contract': 'contracts/check_authority_contract.sh',
    'release-metadata': 'contracts/check_release_metadata.sh',
    'test-layout': 'contracts/check_test_layout.sh',
  }
  foreach name, program : contract_tests
    test(
      name,
      find_program(program),
      args: [meson.project_source_root(), meson.project_version()],
      suite: 'contract',
    )
  endforeach
endif
