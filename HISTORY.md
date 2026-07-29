# History

0.1.0 - 2026-07-29
------------------

Initial native check authority.

- Admit exact transaction check nodes only through their unique
  build-before-check predecessor.
- Require a successful matching `libpkgbuild` result.
- Retain the source-owned check program and exact check-input trees.
- Seal typed passed and failed terminal check evidence.
- Keep resource preparation, process execution, controller progression, and
  legacy recipe compatibility outside the library.
