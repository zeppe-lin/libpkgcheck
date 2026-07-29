# Maintaining libpkgcheck

The authoritative release inputs are:

- `libpkgbuild >= 2.0.0`;
- `libpkgtransaction >= 2.0.0`; and
- `libcrypto` for domain-separated SHA-256 identities.

For every release:

1. qualify clean shared and static closures;
2. compile every public header independently;
3. run request and result refusal tests;
4. inspect the generated pkg-config dependency floors;
5. inspect the shared-object SONAME and `DT_NEEDED` closure;
6. run `git diff --check` and `git fsck`; and
7. tag only the exact tested commit.

`check_request` retains transaction and build public values by value. Any ABI
break in either dependency may therefore require a corresponding SONAME review
here even when this repository's own source is unchanged.
