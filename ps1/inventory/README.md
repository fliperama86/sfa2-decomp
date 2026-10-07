# Function inventory

The static sweep of the PS1 executable and its overlay modules: function
addresses and sizes, no bytes. `ps1/tools/coveragemap.py sweep` writes these
three tables from the executable and the extracted archives with the project's
settings; the [overlay map](../docs/overlays.md) and the
[library families](../docs/library-families.md) describe the sweep and its
limits. Everything here is an estimate: a function is established only when
the matching build rebuilds it.

- `game.tsv`, `library.tsv`: the resident executable, in the columns of
  `families.py --out` and `--library-out`.
- `modules.tsv`: every distinct content of a slot, in the columns of
  `pac.py functions --out`.

The repository's workflow draws the [coverage map](https://fliperama86.github.io/sfa2-decomp/)
from these tables and `src/build.toml` on every push to `main`. Run `sweep`
again when the symbol file or the configuration moves a boundary;
`coveragemap.py render` refuses a stale inventory.
