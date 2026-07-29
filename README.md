# libpkgcheck

`libpkgcheck` is the native, executor-neutral package check authority for
Zeppe-Lin.

It admits one exact transaction `check` node only after the corresponding
transaction `build` node has produced a successful `libpkgbuild` result. The
sealed request retains the transaction-program identity, both phase nodes, the
complete build result, the exact source-owned check program, and the projected
check-input trees.

The library also seals terminal passed or failed results against typed execution
and failure evidence identities. It does not prepare filesystem resources,
construct a process environment, execute the program, interpret backend output,
or advance a transaction controller.

## Build

```sh
meson setup build \
  -Ddefault_library=shared \
  -Dlink_mode=shared \
  -Dman_pages=enabled
meson compile -C build
meson test -C build --print-errorlogs
```

Shared and static dependency closures are separate builds. See `TESTING.md`.
