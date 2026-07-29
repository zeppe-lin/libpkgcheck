# Testing libpkgcheck

The qualification target is the public authority boundary, not only individual
value constructors.

The request suite constructs a real source catalog, resolution result, and
transaction program, then admits a real successful `libpkgbuild` result. It
proves:

- unique build-before-check phase binding;
- exact selected package, release, source, architecture, and program binding;
- rejection of non-check nodes;
- rejection of failed construction;
- rejection of foreign or changed source authority;
- deterministic request identity; and
- exact check-input projection.

The result suite proves passed and failed terminal evidence, classification
identity, and canonical SHA-256 validation.

Run both linkage modes independently:

```sh
./ci/qualify.sh shared
./ci/qualify.sh static
```

Do not combine static `libpkgcheck` with shared dependency authorities, or the
reverse. A successful compile against mixed package-generation ABIs is not a
valid qualification result.
