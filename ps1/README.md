# PlayStation target

Primary working target: Japanese Street Fighter Zero 2, `SLPS_004.15`.
See the [matching pilot](docs/matching-pilot.md) and root [plan](../PLAN.md).

Private files:

- `local/matching-pilot/`: typed C, compiler candidates, build/comparison scripts,
  test harness, successful/failed builds, results and provenance.
- `local/audit/`: pinned main executable, selected disc extracts, inventories,
  initial Ghidra analysis and original load mapping.
- `local/ghidra/ps1.gpr` and `ps1.rep/`: copied analysis project.

From the repository root, offline verification of the existing exact build:

```sh
.venv/bin/python ps1/local/matching-pilot/scripts/compare.py \
  ps1/local/matching-pilot/build/final/metrics.elf
.venv/bin/python ps1/local/matching-pilot/scripts/test_metrics.py
```

To produce a fresh build with the existing external tool setup:

```sh
P=ps1/local/matching-pilot
ssh -M -S "$PWD/$P/ssh.sock" -o ControlPersist=300 -fN dudu@supernova.local
.venv/bin/python "$P/scripts/build.py" "$P/metrics.c" reproduced --compiler 263
ssh -S "$PWD/$P/ssh.sock" -O exit dudu@supernova.local
```

The documented `--compiler 263` is required for the matching parent routine;
the experimental script's default GCC 2.7.2 is a nonmatching comparison case.
The remote compiler stays in the existing isolated user Tools directory.
No credentials are written into the repository.

Original disc images remain in the user's game library. Copied extracts and
Ghidra databases are self-contained relative to this repository; historical
metadata can still name their old extraction locations as provenance.
