# PlayStation target

Primary working target: Japanese Street Fighter Zero 2, `SLPS_004.15`.
See the [matching build](docs/matching-build.md), the
[matching guide](docs/matching-guide.md), the [overlay map](docs/overlays.md), the [matching pilot](docs/matching-pilot.md)
and the root [plan](../PLAN.md).

Public tools, free of game data:

- `tools/matchbuild.py`: whole-image build, comparison and report.
- `tools/fndiff.py`: instruction diff of one unit against the baseline.
- `tools/structgen.py`: struct layouts from a field table, with layout check.
- `tools/mergeunits.py`: merge per-unit work directories.
- `tools/test_matchbuild.py`, `test_structgen.py`, `test_mergeunits.py`: controls.
- `tools/baseline.py`: disc and file manifest: pin, verify, extract.
- `tools/pac.py`: chunk archives: list, extract, scan for code, compare with
  the loader's destination tables, compare the two sides' blocks, inventory
  the functions inside the modules, sort the functions of the executable
  that an inventory does not list.
- `tools/families.py`: which game functions of the executable call which
  part of the Sony library, by `src/library-families.toml`.
- `tools/librefs.py`: the names by which the reference-derived library
  units of a finished build refer to library functions.
- `tools/funcscan.py`: function boundaries in MIPS code by a linear sweep,
  and a comparison of them with an inventory.
- `tools/test_disc_tools.py`: controls for the two disc tools.
- `tools/test_funcscan.py`: controls for the sweep.
- `tools/test_families.py`: controls for the family labels.
- `tools/test_librefs.py`: controls for the names by reference.
- `tools/verify_cc1_golden.py`: compare a compiler against saved reference output.
- `tools/test_verify_cc1_golden.py`: controls for that checker.

Reconstructed source, published:

- `src/`: reconstructed C, `build.toml`, `symbols.ld`, `types.fields`, the
  shared headers, and the Sony library part in `src/sdk/` (see its README for
  where that part comes from and its license).
- The source of overlay modules is published there too, by the owner's
  decision of 2026-10-06, in one folder per module image: `src/slot2a/`,
  `src/slot2b/`, `src/slot00/`, `src/slot0b/`, `src/slot12/` and
  `src/slot16/`. The images `slot17`, `slot08` and `slot2c` are linked
  from the units of `slot16`, `slot00` and `slot2b` and have no folder.

Needed to build and not in the repository:

- `audit/files/SLPS_004.15`: your own copy of the executable. The build
  checks its hash.
- `toolchain/cc1-psx-26`: a GCC 2.6.3 compiler for the PlayStation target.
  The build checks its hash.
- `extract/PAC/BOSS00.PAC`, `extract/PAC/CONT00.PAC`,
  `extract/PAC/PL00X.PAC`, `extract/PAC/PL09.PAC`, `extract/PAC/PL09X.PAC`,
  `extract/PAC/PL0E.PAC`, `extract/PAC/PL0EX.PAC`, `extract/PAC/PL11.PAC`
  and `extract/PAC/PL11X.PAC`: your own copies of those archives from the
  disc. The build takes the module images of slots `0x2a`, `0x12`, `0xb`,
  `0x0`, `0x8`, `0x2b`, `0x2c`, `0x16` and `0x17` from them and checks
  each chunk's hash.
- `src/sdk/include/`: the SDK headers of the sotn-decomp project, copied
  unchanged from the commit named in `src/sdk/README.md`.
- A checkout of `maspsx` at the commit pinned in `src/build.toml`, MIPS
  binutils and `clang` as preprocessor.

Private files:

- `local/open/`: candidates that do not match yet, with their residuals.
- `build/` and `local/build/`: generated objects, linked image and reports.
- `local/baseline/manifest.json`: per-file hashes derived from the disc image.
- `local/baseline/pac-inventory.json`: chunk and code inventory of the archives.
- `local/extract/`: disc files copied out of the image, hash-checked.
- `local/toolchain/`: native compiler, its build script and golden outputs.
- `local/audit/`: pinned main executable, selected disc extracts, inventories,
  initial Ghidra analysis and original load mapping.
- `local/ghidra/ps1.gpr` and `ps1.rep/`: copied analysis project.
- `local/matching-pilot/`: historical pilot evidence. Read-only.

From the repository root:

```sh
.venv/bin/python ps1/tools/matchbuild.py
.venv/bin/python ps1/tools/test_matchbuild.py
.venv/bin/python ps1/tools/baseline.py verify \
  --manifest ps1/local/baseline/manifest.json --files ps1/local/audit/files
```

The first command needs no network. To repeat the build with the pinned
reference compiler, open an SSH control master first and close it afterwards:

```sh
S=ps1/local/toolchain/ssh.sock
ssh -M -S "$PWD/$S" -o ControlPersist=300 -fN <user>@<host>
.venv/bin/python ps1/tools/matchbuild.py --reference --tag reference
ssh -S "$PWD/$S" -O exit <user>@<host>
```

Host and paths are in the private `build.toml`. No credentials are written
into the repository or into any script.

Original disc images remain in the user's game library. Copied extracts and
Ghidra databases are self-contained relative to this repository; historical
metadata can still name their old extraction locations as provenance.
