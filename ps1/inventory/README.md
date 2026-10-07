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
- `contents.tsv`: for every content of `modules.tsv`, its first archive,
  slot, number of archives and the archives that carry it. The map names a
  block from it: the archive family, `END`, `CONT`, `CDEMO`, the stem
  without its two-digit number; the stems themselves when three archives
  or fewer carry the content, `PL11+PL13`; the first family and how many
  others when more than two do, `BOSS+2`. The slot and the archive count
  are in the block's tooltip.

The repository's workflow draws the [coverage map](https://fliperama86.github.io/sfa2-decomp/)
from these tables and `src/build.toml` on every push to `main`. Run `sweep`
again when the symbol file or the configuration moves a boundary.
`coveragemap.py render` has one guard: it refuses a declared function that
no row here touches. A row stale within its range, shorter or longer than
the function it holds, passes; the tables are not checked against the game
files by the workflow.
