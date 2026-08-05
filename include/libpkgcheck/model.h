// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

/*! \file model.h
 *  \brief Executor-neutral check values.
 */
#pragma once

#include <string_view>
#include <vector>

#include <libpkgbuild/request.h>

#include <libpkgcheck/identity.h>

namespace pkgcheck {

enum class check_outcome { passed, failed };
enum class check_failure_kind { execution_unavailable, program_failed, cancelled };

[[nodiscard]] std::string_view to_string(check_outcome value) noexcept;
[[nodiscard]] std::string_view to_string(check_failure_kind value) noexcept;

/*! \brief Exact logical check inputs projected from build authority. */
class check_input_set final {
public:
  [[nodiscard]] static check_input_set project(
      const pkgbuild::build_request& build);
  [[nodiscard]] const std::vector<pkgbuild::build_input>&
  inputs() const noexcept;
  [[nodiscard]] const check_input_set_identity& identity() const noexcept;
  friend bool operator==(const check_input_set& lhs,
                         const check_input_set& rhs) noexcept;
  friend bool operator!=(const check_input_set& lhs,
                         const check_input_set& rhs) noexcept;
private:
  check_input_set(
      std::vector<pkgbuild::build_input> inputs,
      check_input_set_identity identity);
  std::vector<pkgbuild::build_input> inputs_;
  check_input_set_identity identity_;
};

} // namespace pkgcheck
