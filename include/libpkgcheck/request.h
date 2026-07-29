// SPDX-FileCopyrightText: 2026 Alexandr Savca
// SPDX-License-Identifier: GPL-3.0-or-later

/*! \file request.h
 *  \brief Sealed transaction-bound check requests.
 */
#pragma once

#include <libpkgbuild/result.h>
#include <libpkgtransaction/program.h>

#include <libpkgcheck/model.h>

namespace pkgcheck {

/*! \brief Complete immutable authority for one transaction check node. */
class check_request final {
public:
  [[nodiscard]] static check_request seal(
      const pkgtransaction::transaction_program& transaction,
      const pkgtransaction::transaction_node_identity& check_node,
      pkgbuild::build_result build);

  [[nodiscard]] const pkgtransaction::transaction_program_identity&
  transaction() const noexcept;
  [[nodiscard]] const pkgtransaction::transaction_node&
  build_node() const noexcept;
  [[nodiscard]] const pkgtransaction::transaction_node&
  check_node() const noexcept;
  [[nodiscard]] const pkgbuild::build_result& build() const noexcept;
  [[nodiscard]] const pkgsource::program& program() const noexcept;
  [[nodiscard]] const check_input_set& inputs() const noexcept;
  [[nodiscard]] const check_request_identity& identity() const noexcept;

  friend bool operator==(const check_request& lhs,
                         const check_request& rhs) noexcept;
  friend bool operator!=(const check_request& lhs,
                         const check_request& rhs) noexcept;
private:
  check_request(
      pkgtransaction::transaction_program_identity transaction,
      pkgtransaction::transaction_node build_node,
      pkgtransaction::transaction_node check_node,
      pkgbuild::build_result build,
      check_input_set inputs,
      check_request_identity identity);

  pkgtransaction::transaction_program_identity transaction_;
  pkgtransaction::transaction_node build_node_;
  pkgtransaction::transaction_node check_node_;
  pkgbuild::build_result build_;
  check_input_set inputs_;
  check_request_identity identity_;
};

} // namespace pkgcheck
