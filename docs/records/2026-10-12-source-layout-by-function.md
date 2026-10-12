## PS1 source laid out by what it is (2026-10-12)

The owner asked for the source to be organized in a way that is friendly to a
human. The files of `ps1/src/` were named by the address a group of functions
starts at (`d189c4_r1.c`, `slot04_00_0008_r1.c`). They are now in folders by
what the code is, and each file is named after the first function it holds.

- Main executable: `resident/<subsystem>/`, eleven subsystems of a fixed set
  (fighters, objects, script, graphics, hud, menus, input, disc, sound,
  system). The subsystem of each of the 810 files was decided from the names
  of its functions and, for about 250 files, from the code; 176 are low
  confidence. A wrong placement is a `git mv` and a `source` line.
- Overlays: `overlays/characters/<name>/`, `overlays/stages/<name>/` and one
  folder for each screen. Character names come from the name table in the
  executable (its order matches the numbers the code uses); a stage has the
  name of the character with the same number. The screens and the extra
  modules are named from how the code loads them, some with low confidence;
  the evidence of each name is in the commit's working notes, not original
  symbols.
- `ps1/docs/source-map.tsv` gives each unit's earlier path. `ps1/src/README.md`
  describes the layout. The history and the older records keep the old file
  names.
- `ps1/tools/renamefuncs.py` renames a unit's file together with the function
  it is named after (new controls in `test_renamefuncs.py`); without that,
  every rename of a first function would have been refused.
- Include paths in the moved files got the right number of `../`. Nothing
  else in the C changed; units, their names, addresses and flags are as they
  were. Comments in the port and one page that named a file were updated.

What ran. On this Mac the pinned compiler is replaced by the macOS build of
GCC 2.6.3 (its sha is not the pin's), which crashes on about 280 units, so
those could not be compiled here:

- every image except the 7 that hold a crashing unit was built before and
  after the move: 49 pass with the same bytes as the original, the same
  checksums before and after, 3,789 unit objects byte for byte identical;
- for all 3,917 moved units the preprocessed text the compiler receives is
  identical before and after (`clang -E -P` with the build's flags);
- `test_renamefuncs.py` (17 cases), `test_namestats.py`, `test_coveragemap.py`,
  `test_mergeunits.py`, `test_librefs.py`, `port/tools/test_hostbuild.py` pass.
  `test_families.py` (4 cases) and `tools/ai_workflow/tests/test_workflow.py`
  (4 failures, 11 errors) fail the same way on the unchanged commit.

Not run: the Linux compiler of the pin, the image of the 7 images with a
crashing unit, the port's program.
