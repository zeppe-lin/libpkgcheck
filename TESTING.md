# Testing libpkgcheck

The suite is organized by evidence role rather than by historical implementation
file. `unit` covers check-owned scalar values and identities, `integration`
qualifies the real `libpkgtransaction` -> `libpkgbuild` -> `libpkgcheck`
authority seam, `header` compiles every public header independently, and
`contract` checks static repository and boundary invariants.

## Unit

Unit qualification pins all five SHA-256 identity domains, canonical lowercase
encoding, the complete error vocabulary, and the check outcome/failure strings.

## Integration

Input-projection qualification constructs real catalog, resolver, transaction,
and build authority containing both build and check requirements. It proves that
`check_input_set` retains only exact resolver-backed check inputs, preserves their
canonical build-input order, and changes identity when the logical check-input
set changes.

Request admission proves the unique build-before-check relationship, exact
build/check nodes, source-owned program, successful build requirement, wrong-node
refusal, and foreign transaction-node refusal.

Build-authority qualification independently attacks changed source authority and
a successful build from another resolution context whose visible release,
source, architecture, and program fields are otherwise identical. Admission
requires both the exact resolver selection identity and exact resolution-result
identity retained by the transaction; matching visible source fields are not a
substitute for matching authority.

Request-identity qualification proves deterministic sealing and sensitivity to
the exact successful build result and caller-owned build policy while keeping
logical check-input authority stable. Result qualification separately proves
passed evidence and all three semantic failure classes, including identity
sensitivity to request, execution evidence, failure class, and failure evidence.

`libpkgcheck` does not execute checks in this suite. The caller-side
`libpkgcheck-exec` suite separately proves translation from concrete
`libpkgexec` evidence into these check results; this repository proves the
callee authority that executor consumes.

## Headers and contracts

Every installed header is compiled independently. Contract tests pin the
transaction/build boundary, absence of filesystem/process authority, release
metadata, and the test-role layout itself. Header test names use Meson-safe
`header-...` names rather than colon-bearing names.

Run roles independently when diagnosing a failure:

```sh
meson test -C build --suite unit --print-errorlogs
meson test -C build --suite integration --print-errorlogs
meson test -C build --suite header --print-errorlogs
meson test -C build --suite contract --print-errorlogs
```

Run both linkage modes independently for release qualification:

```sh
./ci/qualify.sh shared
./ci/qualify.sh static
```

Do not combine static `libpkgcheck` with shared dependency authorities, or the
reverse. A successful compile against mixed package-generation ABIs is not a
valid qualification result.
