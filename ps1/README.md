# PlayStation target

Primary working target: Japanese Street Fighter Zero 2, `SLPS_004.15`.
See the [matching build](docs/matching-build.md), the
[matching pilot](docs/matching-pilot.md) and the root [plan](../PLAN.md).

Public tools, free of game data:

- `tools/matchbuild.py`: whole-image build, comparison and report.
- `tools/test_matchbuild.py`: negative controls for the build tool.
- `tools/baseline.py`: disc and file manifest, pin and verify.
- `tools/verify_cc1_golden.py`: compare a compiler against saved reference output.

Private files:

- `local/src/`: reconstructed C, `build.toml`, `symbols.ld`.
- `local/build/`: generated objects, linked image and reports.
- `local/baseline/manifest.json`: per-file hashes derived from the disc image.
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
