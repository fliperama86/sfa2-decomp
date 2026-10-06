# Windows reference research

The Windows version is retained for comparison with PS1, not as the current
primary delivery target. See the [gameplay pilot](docs/gameplay-pilot.md).

- `local/gameplay-pilot/`: readable C, build/test/mutation scripts and results.
  The C is source the project wrote. The owner decided on 2026-10-06 that
  all such source is published. This folder has not had the review that
  comes before a first push, so it is still ignored by Git.
- `local/audit/`: hash-pinned `ALPHA2.EXE`, PE metadata and Ghidra evidence.
- `local/cd-audit/`: original-CD versus GOG executable comparisons.
- `local/ghidra/windows.gpr` and `windows.rep/`: copied analysis project.

From the repository root:

```sh
windows/local/gameplay-pilot/build.sh
.venv/bin/python windows/local/gameplay-pilot/test_metrics.py \
  --cases 1024 --helpers 2048 \
  --out windows/local/gameplay-pilot/migration-results-O0.json
.venv/bin/python windows/local/gameplay-pilot/test_metrics.py \
  --elf windows/local/gameplay-pilot/build/metrics-O2.elf \
  --cases 1024 --helpers 2048 \
  --out windows/local/gameplay-pilot/migration-results-O2.json
```

The old experiment's virtual environment was not copied. All tests use the new
root `.venv`; the mutation script uses the Python interpreter that invoked it.
VC5 binaries and their smoke-test evidence remain in the existing isolated
directory on Supernova, as documented in the report. No game matching with VC5
has yet been performed.
