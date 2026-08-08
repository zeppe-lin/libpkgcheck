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
The build result must retain the exact resolver selection and resolution result
used by the transaction, and must realize that exact package release, source
snapshot, build architecture, target architecture, and source-owned check
program. A build from another resolution context is not interchangeable merely
because its visible source fields happen to match.

The source snapshot remains the owner of program bytes. `libpkgcheck` does not
copy them into a second declaration format. The retained transaction node
provides the program accessor, while the build result proves which source and
package artifact were constructed.

## Check inputs

`libpkgbuild` already seals build and check requirements into one complete build
input set. `check_input_set::project()` retains only entries whose exact scope is
`pkgbuild::input_scope::check`.

The projection does not resolve requirements, discover host paths, inspect
artifacts, or construct mount points. Each retained entry is the exact
resolver-backed `pkgbuild::build_input` already sealed by the build request.
Concrete resource identities and host paths are admitted later by
`libpkgcheck-exec`; the core issues no package-tree or materialization authority.

## Result authority

A passed result retains:

- the exact check request; and
- one typed check-execution evidence identity.

A failed result additionally retains one failure classification and one typed
failure-evidence identity. The result model defines three failure classes:

- execution unavailable;
- program failed; and
- cancelled.

The future execution adapter owns translation from backend-neutral process
evidence into these terminal classes. The core does not infer pass or failure
from exit status, signals, logs, cleanup, or backend diagnostics.

## Identity domains

The first-generation protocols define separate SHA-256 domains for:

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

The current core has no:

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
