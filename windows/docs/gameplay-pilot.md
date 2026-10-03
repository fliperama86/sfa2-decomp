# Windows gameplay C feasibility pilot

Date: 2026-10-03. Follow-up to the
[binary investigation](../../research/target-investigation.md).

Later comparison: the [PS1 pilot](../../ps1/docs/matching-pilot.md) rebuilt the apparent
counterpart of this family byte-for-byte. The Windows results below remain
behavioral evidence; historical VC5 matching has not yet been tested.

## Result

**The bounded test supports further investment in Windows.** A connected
three-function game-table family was reconstructed as readable, typed C and
passed differential execution tests against the original x86 instructions.
The first C candidate passed without inline assembly or register-allocation
workarounds. The experiment took about 11 minutes, including local harness
setup and negative controls, but excluding the preceding binary audit and this
write-up.

This establishes practical C recovery for this family, not that the entire
Windows game is easier than CPS2. The input states are synthetic; the routines'
precise gameplay meaning has not been confirmed in a running game. This is
retained as Windows reference research in the separate SFA2 repository, not as
implementation progress on the old CPS2 workbench.

## Selected family

The reference is the same hash-pinned Windows executable found in both the
user's GOG installation and original CD:

```text
a0df7fbd121814927a4a751b7f1f98d02da8ec2db625a739a2761fc9f458760b
```

| Entry | Original bytes | Working interpretation |
| --- | --- | --- |
| `0x004dff40` | 588 | Build twelve per-player frame/table metric records |
| `0x004e018c` | 59 | Compute a signed delta from a six-byte table record |
| `0x004e01c7` | 72 | Compute a signed delta from a thirty-two-byte table record |

Total: 719 bytes and 206 instructions. This is a closed call family: the parent
calls the two helpers, with no external callees or Windows APIs. Its callers
include character-state initialization/change paths, rather than the Windows
launcher. These roles and all new names remain static-analysis hypotheses.

The slice includes two player-side paths, a twelve-slot loop, terminating
sequence scans, table indirection, signed arithmetic, narrowing conversions,
conditional helper calls, and maximum selection. It is more substantial than
three unrelated trivial leaves, but is not a full combat subsystem or dispatcher.

The local C file is 81 lines including types, offset/size assertions, and
comments. Unknown fields remain padding. Original absolute global addresses
are retained. No source or pseudocode containing game-derived implementation
is added to Git.

## Execution test

Clang 22.1.1 and LLD 22.1.1 compiled the same source twice, at `-O0` and `-O2`,
targeting freestanding 32-bit x86 with `-march=i386`. This is a routine-testing
artifact, not a rebuilt Windows application or a claim to use Capcom's compiler.

Unicorn 2.1.4 executed both the original family and each compiled family from
identical initial states. All three members execute their actual machine code;
there are no mocked callees and no calls back into original code from the C run.
The emulator refuses control flow outside the selected family.

For each optimization level:

- **5,120 comparisons passed:** 1,024 whole-family cases and 2,048 direct cases
  for each helper. The same cases were replayed for both builds, not 10,240
  distinct inputs.
- All **206 original instructions** executed. Both outcomes of all ten
  data-dependent conditional branches were reached. An eleventh branch tests
  an immediately loaded constant and has an impossible taken outcome; that
  outcome is explicitly reported as uncovered, not counted as tested.
- Cases cover both sides and other nonzero side values, active/inactive frame
  records, multiple sequence lengths, zero/nonzero box selectors, signed
  boundaries, index truncation, and selected read-only table aliasing.
- Comparisons include the complete fixture memory and three relevant global
  pages, helper return values, callee-saved registers, balanced stack, and
  preservation of the caller's stack beyond argument slots. Write guards reject
  changes outside declared object/output fields and permitted stack storage.

**Eight deliberately incorrect C variants were detected**, including wrong
signs, signedness, index masking, player selection, maximum selection, and a
skipped slot. Each failed with a real result mismatch, not a compiler failure
or unrelated harness exception.

Inputs require valid, initialized pointers and terminating sequences. They
exclude overlap between input tables and writable outputs/active stack,
interrupts, and concurrent mutation. Scratch registers, flags, and dead stack
storage are outside the selected calling contract. Original callees may modify
their stack argument copies, so these are not mistaken for caller-state damage.

## Exact matching and evidence limits

**None of the three compiled functions is byte-identical to the original.**
Modern Clang was used deliberately to answer the behavioral/readability question
quickly. Historical compiler identification and matching remain separate work.

The main practical findings were:

- Ordinary typed C, normal calls, and explicit integer widths were sufficient
  for the tested family. No 68000-style register bridges were required.
- Ghidra was useful, but not authoritative: one apparent extra call argument
  was rejected after checking the original argument pushes. The original selects
  a frame pointer once before its sequence scan; the reconstruction preserves
  that behavior rather than substituting a more intuitive algorithm.
- Initial tests passed with the first source candidate. The compiler target was
  subsequently tightened to i386 and the full suite rerun successfully. A
  textual mutation-generator ambiguity was caught by its uniqueness assertion
  and corrected; it was not a game-code mismatch.

This is not a controlled Windows-versus-CPS2 productivity benchmark: the
routines, prior preparation, and harnesses differ. It also does not establish
live gameplay reachability, complete alias/path coverage, emulator-independent
equivalence, original names, or whole-game correctness. The existing CPS2 pilot
also demonstrated successful behavioral C reconstruction, so passing alone is
not evidence that CPS2 is infeasible.

**Historical recommendation (superseded by the PS1 matching result):** favor
Windows for the next integrated gameplay experiment.
The remaining decision risk is recovering meaningful state and interfaces in a
live game, not whether this x86 family can be expressed and tested in readable C.

## Reproduce locally

Private files live in ignored `windows/local/gameplay-pilot/`:

- `CONTRACT.md`, `metrics.c`, and `original/`: scope, reconstruction, and review.
- `build.sh`, `test_metrics.py`, `mutate.py`, and `requirements.txt`: executable
  experiment and pinned Python dependencies.
- `results-O0.json`, `results-O2.json`, `mutation-results.json`,
  `byte-comparison.json`, and `experiment.json`: results, hashes, and timing.

Run from the repository root with the existing local reference and tool paths:

```sh
windows/local/gameplay-pilot/build.sh
.venv/bin/python windows/local/gameplay-pilot/test_metrics.py \
  --cases 1024 --helpers 2048 \
  --out windows/local/gameplay-pilot/migration-results-O0.json
.venv/bin/python windows/local/gameplay-pilot/test_metrics.py \
  --elf windows/local/gameplay-pilot/build/metrics-O2.elf \
  --cases 1024 --helpers 2048 \
  --out windows/local/gameplay-pilot/migration-results-O2.json
.venv/bin/python windows/local/gameplay-pilot/mutate.py
```

The original executable is never modified. No game launch, remote installation
change, or CPS2 baseline/source ownership change was needed.

## Historical compiler setup follow-up (2026-10-03)

The user attempted the Visual C++ 5 installer on Supernova, which requested
Internet Explorer **3**, not 4. The old InfoViewer help system used IE3
([archived Microsoft VC5 FAQ](https://jeffpar.github.io/kbarchive/kb/167/Q167654/)).
There is no need to install that browser to run the command-line compiler.

The mounted `W:` volume, label `VC50PROCD1`, contains uncompressed tools.
A minimal standalone toolchain now lives outside the repository at
`C:\Users\dudu\Tools\sfa2-vc5-7022` on Supernova:

- `bin`: `CL.EXE`, `C1.DLL`, `C1XX.DLL`, `C2.EXE`, `LINK.EXE`, `LIB.EXE`,
  and `DUMPBIN.EXE`, copied from `DEVSTUDIO\VC\BIN`.
- `bin`: `MSPDB50.DLL` and `MSDIS100.DLL`, from `DEVSTUDIO\SHAREDIDE\BIN`,
  plus `MSVCP50.DLL` from `DEVSTUDIO\VC\REDIST`.
- `lib`: `KERNEL32.LIB`, from `DEVSTUDIO\VC\LIB`.
- The media's `DEVSTUDIO\LICENSE.TXT` is preserved at the toolchain root.

Compiler banner: **11.00.7022**. Linker banner: **5.00.7022**.
All eleven tool/library files were SHA-256 compared against the mounted media.
`manifest.json` and `provenance.json` at the remote toolchain root record
versions, sizes, hashes, and source paths. This checks copy integrity, not
independent authentication of the installation media.

The synthetic `smoke\smoke.c` compiles an increment function and a custom
entry point that exits successfully only when `increment(41) == 42`.
Compilation, linking, actual native Windows execution, and object-header
inspection all returned exit code zero. Results and logs are in `smoke\`.
Reproduce from a batch file or a disposable Command Prompt:

```bat
setlocal
set "ROOT=C:\Users\dudu\Tools\sfa2-vc5-7022"
set "PATH=%ROOT%\bin;%PATH%"
cd /d "%ROOT%\smoke"
cl.exe /c /Od /Fosmoke.obj smoke.c
link.exe /subsystem:console /entry:entry /nodefaultlib /out:smoke.exe smoke.obj "%ROOT%\lib\KERNEL32.LIB"
smoke.exe
echo Program exit: %ERRORLEVEL%
dumpbin.exe /headers smoke.obj
endlocal
```

This is a minimal headerless, CRT-free smoke test, not a full SDK/IDE install
or a game rebuild. The initial linker/dumpbin attempt failed with a missing-DLL
status; copying both `MSDIS100.DLL` and its `MSVCP50.DLL` dependency alongside
the tools resolved it. No system DLLs, registry settings, persistent PATH,
browser components, or game files were changed. The running installer was
not manipulated. Compiler binaries remain external and must not enter Git.

**VC5 is now usable for matching experiments, but has not yet been shown to
match an SFA2 function.** The game's linker version is still a hypothesis
lead, not proof of its exact compiler or flags.
