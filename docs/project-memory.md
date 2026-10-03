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

## Ninth group: third sweep

- Eight batches, 188 small functions attempted, 179 exact: 501 functions and
  49,596 bytes. 55 parked in total.
- One field conflict at merge: a pointer typed as bytes by one batch and as a
  pointer to an object by four others. The single use was rewritten to member
  access through the object pointer and stayed exact.
- A recurring object field at one offset is declared unsigned 16-bit, yet
  different functions need it read as a signed halfword or as a single byte to
  match. The casts work, but the field's true type is still open.
- After this round about 640 functions under 200 bytes remain,
  close to 58 KB. 450 larger functions hold about 211 KB.

## Tenth group: fourth sweep

- Eight batches, 162 small functions attempted, 138 exact: 639 functions and
  63,900 bytes. 79 parked in total. The merge had no conflict.
- A pattern now parked about ten times: the original takes the high byte of
  a 16-bit value with a shift left by 16 and an arithmetic shift right by 24,
  then stores a byte. Every C spelling tried, including a union split, a
  signed cast and an `int` local, compiles to a plain shift right by 8. The
  hypothesis is that the original uses the shifted value somewhere else as
  a full integer. Failed spellings do not establish what the original
  expression was. Not solved.
- Full rebuilds became slow with more than a hundred units. Banking one batch
  takes over a minute. An object cache keyed on every compile input is in
  progress; link and all checks will still run on every build.
- After this round about 480 functions under 200 bytes remain,
  close to 41 KB. 450 larger functions hold about 211 KB.

## Eleventh group: fifth sweep and the library area

- Eight batches, 206 small functions attempted, 130 exact: 769 functions and
  74,216 bytes. 151 parked in total.
- The upper code area, from about `0x80157000`, is library code, not game
  code. Agents recognised graphics, sound and disc routines by behaviour.
  Many of its C functions match with the same compiler. Three things there do
  not fit the current rules or tools:
  - About thirty functions are hand-written stubs that jump into the BIOS or
    use `syscall` or `break`. C cannot produce them. They need assembly owners,
    which the build does not have yet.
  - Several routines only match when stores to hardware registers are treated
    as volatile. One agent confirmed that on one function. The brief forbids
    `volatile` because it was meant to stop steering tricks. For real
    memory-mapped registers it is the honest construct. Not allowed yet;
    needs a rule change.
  - A few units show a different assembler behaviour: `li` as `addiu` and a
    three-instruction table lookup. That points to library objects assembled
    with a newer assembler than the game code. The build has one assembler
    setting for everything. Needs a per-unit setting. Hypothesis until a unit
    matches with it.
- An agent changed the type of an existing field in the shared table. Its own
  batch built, and an already matched unit silently stopped matching inside
  that batch. The banking helper reported the failing unit, the line was
  restored, and the batch passed. The merge would also have refused it.
- The private parking helper reused a unit name when a unit was split twice
  and overwrote a source file. Four functions that were probably exact were
  lost and go back into the pool. Fixed in the helper.
- After this round about 280 functions under 200 bytes remain,
  close to 26 KB. 450 larger functions hold about 211 KB.

## Twelfth group: sixth sweep and a retry of the parked pool

- Eight batches over the lowest addresses, 163 small functions attempted, 135
  exact. Then every parked function was rebuilt on its own as a single-function
  unit, which recovered 17 more. 921 functions and 87,852 bytes.
  162 parked in total.
- Why functions were parked by mistake: when a unit failed to compile, the
  build stopped before later units had objects, and the banking helper read
  the missing objects as differences. It now parks only the unit that failed
  to build and looks again. The retry is a separate helper and can be repeated
  after any round.
- Three merge conflicts, all resolved mechanically by rebuilding each batch
  with each candidate type: one field typed as a word by a batch whose only
  users of it had been parked, so that line was dropped as unverified; one
  signed-versus-unsigned halfword; and two batches that each invented a
  struct with the same name, one of which was renamed.
- One function needs the assembler's division checks expanded, which the
  build has no option for. Open tool item, with per-unit assembler settings.
- The program entry routine is hand-written assembly.
- After this round about 120 functions under 200 bytes remain,
  close to 10 KB. 450 larger functions hold about 211 KB.

## Thirteenth group: end of the small-function sweep

- Five batches, 117 small functions attempted, 106 exact: 1,027 functions and
  96,944 bytes, close to 30 percent of the resident code bytes.
  173 parked in total, about 20 KB. A second retry of the parked
  pool recovered nothing, which confirms the fixed banking step.
- What the sweep showed overall. Functions under 200 bytes match at roughly
  85 percent with this method. The misses fall into a few repeating kinds:
  one register off, one instruction scheduled early or late, two blocks merged
  or kept apart, an unused stack reservation, and hand-written assembly.
- Remaining in the resident image: 450 functions of 200 bytes or more,
  about 211 KB. The earlier rounds showed this method mostly fails there.
- Next method under test: an automatic permutation search that rewrites a
  near-miss candidate and recompiles until the bytes match. It costs CPU, not
  model tokens. Its results still go through the normal build before they
  count.

## Fourteenth group: medium functions and first permutation results

- Eight batches over functions of 200 to 300 bytes: 70 attempted, 39 exact.
  That is 56 percent, against roughly 85 percent for the small ones. Three
  more came back from the parked pool. 1,069 functions and 106,776 bytes.
  201 parked, about 27 KB.
- **The function table undercounts some functions.** A routine that ends in
  an endless loop is followed by an epilogue the disassembler did not attach
  to it, so the listed size is short by the epilogue. Candidates for such
  functions looked 20 or 24 bytes too long and were parked. The retry helper
  now extends a candidate into the gap when the built code is longer than the
  listed size and still ends before the next function. Two functions matched
  that way. Earlier notes that such originals have no epilogue were wrong.
- **Permutation search, first results.** The open-source permuter runs on the
  real toolchain. Both sides are linked at the function's address, so the
  score sees real immediates, and a result only counts after the normal build
  accepts it. On five hand-picked near misses it found nothing in eight
  minutes each. In a sweep it then matched one function whose candidate
  started far from the target. The form it found adds one extra pointer
  variable; the source carries a comment saying it was machine-found.
  Passes that insert meaningless code are switched off.
- **First function that needs data ownership.** One routine is a switch
  compiled to a jump table. The build rejects any unit with read-only data,
  so it cannot match until the build can own data ranges.
- One routine's residual is that the original loads the same field twice,
  once signed and once unsigned, where the candidate reuses one load. That
  pattern has now appeared several times.

## Fifteenth group: second medium round

- Eight batches over functions of 200 to 300 bytes: 70 attempted, 42 exact.
  1,111 functions and 117,020 bytes. 229 parked, about 33 KB.
- Two type conflicts at merge on one struct, signed against unsigned. Each was
  settled by rebuilding both batches with each type.
- A second switch function is exact in its code and differs only in the
  address of its jump table. Read-only data ownership for the build is being
  added as a separate change.
- One function copies a 39-word local table from data. Its source assigns a
  struct from an extern to get the same copy loop, because the build cannot
  own the initialiser's data yet. That is a stand-in, to be replaced by the
  real initialiser once data ownership exists.
- Two more functions carry an unused local array to reproduce a stack frame
  the original reserves and never uses. That makes four. It matches, and it
  is not an explanation of why the original reserves the space.

## Sixteenth group: last medium functions, first larger ones, first jump table

- 63 functions attempted: 43 of 200 to 300 bytes, mostly library code, and 20
  of 300 to 500 bytes of game code. 39 exact. The larger game functions
  matched 13 of 20. Library code is harder, mainly because of
  hardware-register stores.
- Five more came back from the parked pool. Four were matched by the
  permutation sweep running in the background. One is the first switch
  function, accepted now that the build owns its 24-byte jump table as
  read-only data.
- 1,155 functions and 128,896 bytes. 259 parked, about 43 KB.
- Lessons that helped several functions: a `u8` loop counter removes a stack
  frame that an `int` or `short` counter creates; a loop written with `goto`
  can keep a test-then-decrement order that `do/while` loses; a decrement
  through an `int` temporary gives `addiu -1` where `--` on a halfword does
  not. These are observations on specific functions, not general rules.
- An agent deleted an existing symbol line in its copy, which broke the link
  of an older unit there. Restored by hand. The merge tool does not flag a
  deleted symbol line, because deleting is normal when a unit takes over a
  symbol. A stray backup file from an agent also reached the main private
  tree and was removed.

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
