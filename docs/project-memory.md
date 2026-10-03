# Project memory and handoff

This file is durable project memory. Read it with `../AGENTS.md`, `../PLAN.md`,
and `goals.md` at the start of a new session. Update it when the user corrects
an assumption or a new result changes a decision. Do not depend on chat history
or an assistant-specific memory directory to recover the project's context.

## User preferences and scope

- The user wants understandable SFA2 source, not merely an assembly dump or a
  generic decompiler demo. Readable gameplay C with explicit assembly exceptions
  is acceptable. Do not demand all-C regardless of readability or cost.
- This project originated in `68k-decomp`, initially investigating CPS2. The
  user requested a separate `~/Projects/sfa2-decomp` repository and specifically
  wanted 68k-related implementation left behind. PS1 is the current working
  target; Windows remains useful reference research.
- Migration was verified with fresh pilot rebuilds, comparisons, execution
  checks, and read-only Ghidra reopening. The [migration record](migration.md)
  describes copied artifacts and exclusions; [goals](goals.md) identifies the
  next work package. This repository, not external assistant memory, is the
  authoritative handoff.
- Be concise, answer first, one subject at a time. Avoid excessive identifiers,
  long status recaps, the em dash character, and suggestions about stopping work.
- Work on `main`; branches and PRs need a request. No agent attribution in their
  names/titles. Commit/push when requested, after checking what will be published.
- Always launch MAME windowed, including scripted/headless usage if ever needed.
- User-owned original inputs must remain unchanged. No passwords in files or
  reports. Existing external tools and scoped environments are preferable to
  invasive system changes or unnecessary new VMs.

## Most important correction: matching versus running

The user correctly challenged the proposed next milestone of validating the
matched routines in a running game. **Byte-identical output at the correct
addresses/mapping already establishes instruction identity.** A game boot,
routine injection, rebuilt disc, or new runtime adapter is not another
mandatory correctness gate for that property.

Live observation can answer specific semantic/loading questions; behavioral
tests are important for nonmatching source or changes. Neither is a reason to
hold already exact source behind a redundant playtest gate. Only three routines
match so far, not the full executable. The next work is expanding matching
source ownership and coverage, not generating every source file before a boot.

## Current primary baseline

- PS1 Japanese Street Fighter Zero 2, `SLPS_004.15`, 616,448 bytes.
- Executable SHA-256:
  `0a522fd4791edfe95150edea057ca7113b8d0e93ed085be6854c042012c4a2a2`.
- Disc SHA-256:
  `397f761609f9b2cb82150a533d5ab820a797852a0703346c633df35ff526e255`.
- PS-X EXE header is 2,048 bytes; payload loads at `0x80118900`, entry
  `0x80118908`. Read headers rather than guessing addresses.
- `ps1/local/audit/` has the extracted baseline and previous static analysis.
  Full disc images remain in the user's game library; their locations are in
  the private inventory. Ghidra's project is `ps1/local/ghidra/ps1.gpr`.
- No original game symbols have been recovered. Sony SDK source strings are
  not gameplay symbols. PAC/overlay code inventory remains incomplete.

## PS1 matching result

- Closed resident family: parent `0x80142128` (524 bytes), helpers
  `0x80142334` (36) and `0x80142358` (32). All 592 bytes match from 74 lines of
  typed C at original addresses, without inline assembly or byte blobs.
- Source: `ps1/local/matching-pilot/metrics.c`. Full details:
  `ps1/docs/matching-pilot.md`. All reconstructed code and granular evidence are
  ignored private artifacts, not approved for publication.
- Whole-family SHA-256:
  `51a800c583016f8856d32a3a82440142293a8e7dea0d425190ab9b70d9a9b73a`.
- Compatible compiler is rebuilt GCC 2.6.3, PlayStation target, `-O2 -G0`.
  maspsx commit `e85ecb373828aabea96d7e6400974d4f91ce8c74`, compatibility mode
  `--aspsx-version=2.34`, then GNU MIPS binutils 2.47 for R3000/O32.
- This does not uniquely identify Capcom's original compiler, SDK, flags, or
  assembler version. Other versions/modules may differ. GCC 2.7.2 matches the
  final helpers but not the parent (111/131 words at corresponding offsets).
- The build script's experimental default is GCC 2.7.2: pass `--compiler 263`
  explicitly for the matching build.
- The main natural source changes were an explicit inner loop with exits,
  signed-short slot arithmetic, negated-origin-plus-extent expressions, and
  loading the wide-table base before index addition. No dummy operations or
  assembly constraints were required.
- Clean rebuilding, independent objcopy/dd/cmp and target-layout assertions
  passed. 5,120 bounded execution cases covered all 148 instructions and both
  outcomes of ten conditional branches. Eight source mutations produced actual
  output mismatches and failed the byte gate.
- Roles are inferred frame/box-table metrics, supported by code/callers and
  correspondence to the Windows family. No live-game semantics or whole-game
  correctness claim. Unicorn MIPS32 is not cycle-accurate PS1/R3000/GTE proof.
- The roughly twenty-minute pilot benefited from previous Windows recovery;
  it is not a controlled productivity benchmark or a whole-game time estimate.

## Windows reference

- GOG, original-CD installation, and mounted-CD `ALPHA2.EXE` were verified
  byte-identical using native comparison and SHA-256:
  `a0df7fbd121814927a4a751b7f1f98d02da8ec2db625a739a2761fc9f458760b`.
- Native x86 with apparently unoptimized C/C++ gameplay code. PE linker version
  5.0 is a compiler hypothesis lead, not proof of an exact compiler.
- A surviving NB10 record names an external developer PDB, but the PDB was not
  found in the inspected installed or CD files. Do not claim recovered symbols.
- The apparent same three-function family was reconstructed as typed C and
  tested with Clang 22.1.1 at O0/O2: 5,120 comparisons per build, eight mutations
  detected. Neither build byte-matches. VC5 matching has not been attempted.
- Files are under `windows/local/`; report: `windows/docs/gameplay-pilot.md`.
  Preserve this as a reference, not a second simultaneous main target.
- VC5 setup requested IE3, not IE4. The browser was only needed for old IDE help.
  Command-line compiler/linker tools run separately without installing IE.

## Tool locations and reproducibility

- Python environment: fresh root `.venv`, Python 3.12, dependencies pinned in
  `requirements.txt`: Unicorn 2.1.4, Capstone 5.0.7, pyelftools 0.32.
- Clang: `/opt/homebrew/opt/llvm/bin/clang`; LLD:
  `/opt/homebrew/bin/ld.lld`; MIPS tools: `mipsel-linux-gnu-*` on PATH.
- External maspsx and old-gcc checkouts are in `~/Projects/references/`.
  The SOTN reference supplied compiler-build metadata/checksums, not game code.
- Ghidra: `~/Tools/ghidra_12.1.4_PUBLIC`. macOS ARM64 native decompiler is already
  built. Scope Java 25 to the Ghidra process using `JAVA_HOME`; inherited Java
  may be 17. Do not alter global Java configuration.
- Remote host: `supernova.local`, account `dudu`, default SSH shell `cmd.exe`.
  Existing Ubuntu WSL runs the GCC 2.6.3 Linux compiler. The private remote
  directory remains `C:\Users\dudu\Tools\sfa2-ps1-pilot`.
- Historical compiler archive SHA-256:
  `ab156fe598ccad6aef704b8b35f3272de7edaf7eaab4e3d504c346ce6647df6c`.
  Executed compiler SHA-256:
  `c248a5272dc1a4537b177c71e3d785f48204718e36b076d5d7940b9469a0beb8`.
- VC5 is isolated at `C:\Users\dudu\Tools\sfa2-vc5-7022`. Compiler 11.00.7022,
  linker 5.00.7022. Headers/full CRT libraries/IDE are not installed there.
  The report lists the DLL dependencies and successful synthetic smoke test.
- Use an authenticated SSH master when remote compilation is needed; never put
  passwords into scripts. Authentication may need to be established again in a
  fresh session. Close temporary control connections when finished.

## Mistakes and lessons to preserve

- Ghidra pseudocode is a candidate. Check the actual argument registers/pushes:
  apparent missing/extra helper arguments were corrected from disassembly.
- The frame pointer is selected once before sequence traversal. Do not replace
  the original algorithm with the more intuitive per-step frame update.
- Use Ghidra function-body ranges and call references. Assigning every decoded
  instruction to the nearest preceding start can overcount calls across gaps.
- Old `cc1` expects preprocessed input. Use Clang `-E -P -nostdinc` first.
  Respect C89 declaration ordering and enforce each subprocess's exit status.
- Unary negation and subtraction may select different old-compiler evaluation
  orders even when algebraically equivalent. Keep readable variants and record
  compiler evidence; do not silently patch machine code to make a match.
- Unicorn MIPS execution translates KSEG0 addresses to physical memory, while
  its memory API uses mapped addresses. Normalize those correctly in tests.
  MIPS branch coverage must account for the delay-slot instruction.
- Windows callees may modify their own stack argument copies. Do not mistake
  this for unrelated live caller-stack corruption.
- Mutation replacements must be unique. A substring for `boxes` also matched
  `wide_boxes`; use complete statements and assert replacement counts.
- Interactive Windows SSH commands need carriage return. PowerShell 5 can
  treat redirected native stderr banners as errors with Stop enabled; redirect
  native logs through `cmd /d /c`, then check the actual exit code.
- VC5 link/dumpbin require both `MSDIS100.DLL` and `MSVCP50.DLL` alongside tools.
  Never fix this by overwriting modern system DLLs or installing an old browser.
- The old repo has concurrent unrelated CPS2 changes. Migration is copy-only;
  do not reset, stage, delete, or commit those changes as part of this project.
