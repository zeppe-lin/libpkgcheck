# Contributing

Changes must preserve the authority boundary documented in `DESIGN.md`.

Before submitting a change:

```sh
./ci/qualify.sh shared
./ci/qualify.sh static
```

Public request or result fields require an identity-domain review. New host
paths, process controls, backend objects, or filesystem effects do not belong in
this repository. Put execution realization in `libpkgcheck-exec` instead.

Use focused commits and include tests that prove both admission and refusal
paths. Keep public documentation synchronized with the code in the same series.
