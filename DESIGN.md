# Native package-check authority

## Scope

`libpkgcheck` answers one question:

> Does this successful construction result authorize this exact transaction
> check node, and what terminal check evidence belongs to that request?

The answer is an immutable `check_request` followed by an immutable
`check_result`. The library does not execute either phase.

## Request authority

A request is admitted from three existing authorities:

1. one complete `libpkgtransaction::transaction_program`;
2. one exact node identity naming a `check` action in that program; and
3. one successful `libpkgbuild::build_result`.

Admission requires a unique `build_before_check` phase edge. The predecessor
must be a build node retaining the same resolver selection as the check node.
The build result must realize that exact package release, source snapshot, build
architecture, target architecture, and source-owned check program.

The source snapshot remains the owner of program bytes. `libpkgcheck` does not
copy them into a second declaration format. The retained transaction node
provides the program accessor, while the build result proves which source and
package artifact were constructed.

## Check inputs

`libpkgbuild` already seals build and check requirements into one complete build
input set. `check_input_set::project()` retains only entries whose exact scope is
`pkgbuild::input_scope::check`.

The projection does not resolve requirements, discover host paths, inspect
artifacts, or construct mount points. Each retained entry still carries its
resolved package input and exact materialized tree identity.

## Result authority

A passed result retains:

- the exact check request; and
- one typed check-execution evidence identity.

A failed result additionally retains one failure classification and one typed
failure-evidence identity. Version 0.1 defines three failure classes:

- execution unavailable;
- program failed; and
- cancelled.

The future execution adapter owns translation from backend-neutral process
evidence into these terminal classes. The core does not infer pass or failure
from exit status, signals, logs, cleanup, or backend diagnostics.

## Identity domains

Version 0.1 defines separate SHA-256 domains for:

- projected check-input sets;
- transaction-bound check requests;
- passed check results; and
- failed check results.

A request identity binds the transaction program, unique build and check nodes,
complete build result, and projected check-input set. Program bytes and source
semantics are bound transitively through source, selection, transaction, and
build identities.

Diagnostic origins, host paths, process IDs, backend names, and wall-clock time
are not check semantic authority.

## Deliberate omissions

Version 0.1 has no:

- filesystem or archive materialization;
- execution root, credentials, interpreter, or mount policy;
- process backend;
- environment-variable layout;
- log rendering or persistence;
- retry policy;
- transaction progression mutation;
- durable check journal;
- Pkgfile compatibility or legacy check-function importer.

Those omissions are boundaries, not unfinished methods. Resource realization
and process evidence belong in `libpkgcheck-exec`; controller progression belongs
in `pkgctl`.
