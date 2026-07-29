# Migration

There is no native predecessor to `libpkgcheck 0.1.0`.

Legacy recipe check functions are not admitted directly. A future migration
tool must first produce native `recipe.yml/2` source authority, an explicit
transaction check node, and a successful native build result. This library does
not parse Pkgfiles or reconstruct missing evidence.
