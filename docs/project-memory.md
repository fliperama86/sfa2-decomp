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
- Since 2026-10-03 the user wants a branch and pull request for every change
  unless told otherwise. Open the PR, check it every 60 seconds for the owner's
  comments, apply requested changes, and merge when the owner approves in a
  plain comment (no GitHub review). The repository is public, so only the
  owner's comments count. No agent attribution in branch names or PR titles.
  Check what will be published before every push.
- After each PR is approved and merged, continue with the next item without
  asking. Contact the user only when a decision is needed from them.
- The user wants byte-identical output. Retained raw bytes keep the image
  identical but count as zero progress; only C that compiles to the exact
  original bytes counts.
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

## Matching build and second group

- `ps1/tools/matchbuild.py` is the build entry point. Contract:
  `ps1/docs/matching-build.md`. Private inputs: `ps1/local/src/`. The pilot
  scripts under `ps1/local/matching-pilot/` are historical evidence only.
- The whole resident executable rebuilds byte-identically: eight C functions
  (1,228 bytes) at original addresses, everything else retained raw. Raw bytes
  are scaffolding and are reported separately from recovered C.
- Second group, all exact on the first C candidate: four adjacent setup helpers
  at `0x80130988` to `0x80130b10` and the small caller at `0x80131f7c`. The
  first helper copies five table pointers into the object, which are the box
  tables the metrics family reads. Shared types live in `object.h`.
- Scratchpad holds per-side resource pointers. The right side's set sits 0xb0
  above the left side's. Names in `symbols.ld` are inferred.
- **Assembler correction:** the original expands indexed symbol loads through
  `$at` with `addiu` and a preceding `nop`. That is ASPSX 2.21 or older, not
  2.34. The pilot family contains no such load, so 2.34 matched it by chance.
  Versions 1.07, 2.08 and 2.21 all match the eight functions; a division would
  separate them. The build uses 2.21.
- A native macOS `cc1` now runs the build without the remote host. It was
  built from the old-gcc recipe plus `MASK_GPOPT` in the target defaults, which
  the reference binary reports and that recipe lacks. It emits identical
  assembly on 121 of 131 reference outputs. The ten failures are all
  floating-point constants, wrong because GCC 2.6.3 assumes host words as wide
  as its 32-bit `HOST_WIDE_INT`. The build marks it `no_float` and refuses such
  units. `--reference` rebuilds with the pinned Linux binary.
- The SSH control socket is now `ps1/local/toolchain/ssh.sock`. The user gives
  the password in the session. Feed it to ssh through an askpass helper that
  reads an environment variable, so it never reaches a file or a script.
- Resident code calls addresses above the resident image. They resolve to
  character overlay blocks: see `ps1/docs/overlays.md`.

## Third group: object reset

- `reset_object` at `0x80129020` (948 bytes) and three adjacent helpers are
  exact. Twelve functions, 2,324 bytes. A subagent matched them against the
  build in a separate private config directory; the top level then renamed,
  moved shared types to headers and rebuilt with both compilers.
- Two player objects sit at fixed addresses `0x394` apart. A block of global
  state at `0x80190108` is addressed from one base: declaring it as one struct
  (`GameState` in `game.h`) was required to match. Separate externs for its
  members did not reproduce the original register use.
- The routine zeroes part of the object through a pointer that lives in that
  global block and is stored back on every iteration. A local cursor does not
  match.
- The two handler tables are 24 pointers per side. Entries `0x15` to `0x17` of
  the first table serve three pairings where both players hold the same
  character or its variant. Those numbers match the three character files
  without a second-side twin. The link to those files is inferred.
- **Two names for one address.** The original loads the entry for the last
  pairing in two separate blocks. With one symbol name both compilers merge
  the blocks and the unit comes out 16 bytes short. The source therefore uses
  a second symbol at the same address, with a comment. Hypothesis, not a
  finding: the original source may have reached that address through two
  different expressions. A matching reconstruction does not prove what the
  original expression was. It is the one
  place where the C is shaped by the compiler's merging rather than by a
  recovered meaning. Verified with the reference compiler too.
- The original stores a zeroed local rather than a constant in one run of byte
  stores, then reuses that local. The C keeps that local.
- `ps1/tools/fndiff.py` shows an aligned instruction diff of a unit against
  the baseline. It reads the last build's object, so run the build first.
- Work on a candidate in a sibling copy of `ps1/local/src` with `--config` and
  its own `--tag`. The main tree then stays exact while a PR is under review.

## Fourth group: first parallel round

- Five subagents each matched one unit in its own copy of the private source
  directory. 33 functions became exact; 45 functions and 5,548 bytes in total.
  Three functions did not match and are parked in `ps1/local/open/` with their
  residual and attempt log. They stay raw. Nothing inexact enters the build.
- Shared structs are now a field table, `types.fields`. The build generates
  the header and checks the layout with a second compiler. Hand-written
  padding is gone. `mergeunits.py` merges unit directories and reports every
  disagreement instead of resolving it.
- The merge surfaced two type conflicts on the object struct. Both were real
  knowledge: offset `0x18` is the current sequence step and `0x88` the current
  frame record. One agent had invented a second struct with unsigned fields
  for the sequence step. The existing signed struct matches the same code,
  because a 16-bit copy loads unsigned whatever the field's sign.
- The two player objects behave as one array of two. Code reaches the other
  player as the first plus one or the second minus one.
- **Second use of two names for one address.** One routine stores the address
  of the global state block into a pointer inside that block. The original
  loads the address on its own; with one symbol the compiler derives it from
  the store address. A second symbol at the same address matches. Hypothesis:
  the state block sits inside a larger object with a lower base symbol, which
  would give both effects without a second name. Not verified.
- The pointer inside the state block points back at the block itself in that
  routine. The struct used for its target may be the same type as the block.
  Not unified yet.
- One address is read as a signed byte in one unit and as an unsigned 16-bit
  value in another. Each unit declares its own extern. Meaning unknown.
- Lessons for briefs: give each agent its own scratch folder, and check the
  tool paths in the brief before launch. One tool path was wrong in this round
  and every agent had to find the tool elsewhere.
- Open residuals: a routine that reserves 16 bytes of stack it never uses,
  where two 16-bit locals only produce 8; a routine whose two saved registers
  come out swapped; a routine whose second loop the original strength-reduces
  and the candidate does not.

## Fifth group: second parallel round

- Five more units, merged with no conflict. 25 more functions exact: 70
  functions and 9,836 bytes. Three more functions are parked; six in total.
- The merge was clean this time. Shared `field_<offset>` names avoid name
  conflicts only. Units can still disagree on a field's width, signedness or
  pointer type, and the merge then stops on it.
- Cleanup debt recorded, not yet done: several units name addresses inside the
  global state block, or the two players' fields, as separate symbols or as
  one-field wrapper structs. They match, but they should become members of the
  existing structs. Each change has to be rebuilt and stay exact.
- One function keeps a `goto`. Without it the compiler merges two duplicated
  check chains and the size is wrong.
- In three functions the C that matches uses an uninitialised local or falls
  off the end without a return value. That is a property of the matching
  candidate. It is not proof of a bug in the original source, which would need
  its own caller and data-flow evidence.
- A round can run while the previous PR is in review, as long as the main
  private tree stays at the reviewed state. Merged rounds wait in a sibling
  directory and are promoted after the PR merges.
- Private helpers under `ps1/local/`: `setup_round.py` creates the unit work
  directories from address ranges, `park.py` takes unmatched functions out of
  a unit and splits it around them, `round_brief.md` is the agent brief.
- Parked residual kinds so far: unused stack reservation, swapped saved
  registers, a missing strength reduction, a load that lands in the wrong
  register, register allocation order, and a conditional term the compiler
  turns into a branch.

## Sixth group: third parallel round

- Five units attempted. 21 more functions exact: 91 functions and 13,316
  bytes. Nine more functions are parked; fifteen in total.
- Two units failed completely. Their four functions are 320 to 716 bytes.
  Small functions match on the first or second build. Functions above roughly
  300 bytes often end with the right logic and size but the wrong register
  allocation or instruction order, and repeated source reshaping does not fix
  that. More rounds of the same method will keep parking the large ones.
- Field and symbol additions from a unit that matched nothing are not merged.
  They are unverified. Their field notes are kept next to the parked source.
- Clue for the global state block: one original routine builds the address of
  one member in a register and reaches another member at a negative offset
  from it. Every build here loads each member through its own address. This
  fits the earlier hypothesis that the block is reached through a pointer or
  sits inside a larger object. Not verified.
- Several routines reserve stack space they never use: 8 or 16 bytes more
  than the candidate. Small integer locals of different widths produce 8 in
  some shapes. The cause of the rest is unknown.

## Seventh group: small-function sweep

- Six batches of functions under 200 bytes, picked in address order. 99 more
  functions exact: 190 functions and 22,572 bytes. 28 parked in total.
- **The build decides, not the report.** One agent reported ten functions as
  exact that the build showed were not. Banking is now automatic: a helper
  builds the batch, asks the diff tool about each function, parks what
  differs, and repeats until the build passes.
- **Decrement form matters, narrowly.** On an unsigned byte field, `x--` adds
  minus one while `x -= 1` and `x = x - 1` add 255. That is the only confirmed
  pattern: on an unsigned 16-bit field all three add minus one in a small
  fixture, and so does the explicit form on a signed byte. Inside larger
  functions agents needed other forms, so type and context matter too. The
  guide carries the fixture. Seven parked functions, all byte-field
  decrements written as `x = x - 1`, matched after being rewritten as `x--`.
  The native and the reference compiler gave identical output on the probe
  that was run through both.
- Suspected native-compiler defects must be tested against the reference
  compiler before believing them. This one was not a defect.
- A field typed signed by one batch and unsigned by three others was resolved
  by rebuilding every batch with each type. Unsigned kept all four exact.
- What remains: about 1,000 functions under 200 bytes, close to 90 KB, which
  this method handles. About 450 larger functions hold roughly 210 KB and
  mostly fail with it.

## Eighth group: second sweep

- Eight batches, 150 small functions attempted, 132 exact: 322 functions and
  35,268 bytes. 46 parked in total. The eight batches merged without a field
  or symbol conflict.
- One merge problem the tools did not catch: a unit defined a struct in its C
  file that another batch had added to the field table under the same name.
  Each batch built alone and the merged tree failed to compile. Same layout,
  so the local definition was removed. Units must not define struct types in
  C; the brief now says so.
- An old-style function definition broke the private parking helper, which
  expects prototyped definitions. Handled by hand.
- The residual kinds repeat: one saved or temporary register off, an address
  computed one instruction early, two blocks the compiler merges or does not
  merge. These are candidates for a mechanical permutation search rather than
  for more attempts by hand.

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
- GNU as pads every section to 16 bytes. A unit whose code size is not a
  multiple of 16 then overlaps its neighbour. Assemble with `-no-pad-sections`.
  The pilot family was 592 bytes, a multiple of 16, which hid this.
- A linker-script assignment silently overrides a symbol defined by an object.
  Never list a unit's own function in `symbols.ld`; the build rejects it.
- One matching family is thin evidence for a toolchain setting. The assembler
  version only showed up with different instruction patterns.
- The old repo has concurrent unrelated CPS2 changes. Migration is copy-only;
  do not reset, stage, delete, or commit those changes as part of this project.
