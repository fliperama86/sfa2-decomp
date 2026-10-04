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

## Seventeenth group: larger game functions

- 54 functions of 300 to 500 bytes attempted, 31 exact. One more came back
  from the parked pool through the permutation search. 1,187 functions and
  141,124 bytes, about 43 percent of the resident code bytes.
- **Reconciling the previous round's parked count.** The parked pool went
  from 229 to 259 in the last milestone, 35 additions and 5 recoveries, while
  63 attempts with 39 matches explain only 24 additions. The other 11 were
  functions parked by the first four batches of this round, which had already
  been banked when that snapshot was taken. The parked pool is shared across
  rounds and a snapshot can include a round in progress. From now on the
  milestone notes give the parked count at the moment of the snapshot and
  say which rounds it includes.
- Parked pool at this snapshot: 270 functions, about 48 KB. It includes
  round fifteen batches banked so far, if any. Unattempted: 193 functions,
  about 139 KB, not counting those handed to agents in the running round.
- Success by size so far: about 85 percent under 200 bytes, 58 percent from
  200 to 300, 57 percent from 300 to 500.

## Eighteenth group: end of the game sweep, large functions, SDK identification

Counts at this snapshot. No round is in progress, so they include nothing
unfinished.

- 1,274 functions exact, 176,328 bytes, 68 bytes of read-only data in four
  ranges. About 54 percent of the inventoried resident code bytes. Both
  compilers reproduce the image.
- Rounds fifteen and sixteen, game functions of 300 to 1,000 bytes: 78
  attempted, 50 exact.
- Functions of 1,000 bytes and more, one per agent: 16 attempted, 3 exact.
  The failures are register allocation, loop-invariant hoisting and stack
  frame size, with the logic and usually the size right.
- The permutation search has now tried 107 parked functions and matched 20,
  all of them accepted by the build. Retries against newer trees and the
  division expansion brought back a few more. About 200 parked functions have
  not been tried.
- Parked: 287 functions, about 79 KB (210 game, 77 library). Unattempted: 91
  functions, about 73 KB (6 game functions of 1,700 bytes and more, 85
  library).

SDK library identification:

- A public reconstruction of the Sony SDK libraries exists in the SOTN
  decompilation project (repository under AGPL-3.0, the SDK C files marked
  MIT): `xeeynamo/sotn-decomp` at revision
  `62d03266cc927aabefea3b0605cabc254b48831b`. Nothing from it is committed or
  published here. Private copies do exist in the ignored workspace under
  `ps1/local/src/sdk/`: the 15 imported files from `src/main/psxsdk/<library>/`
  unchanged, each library's internal headers from the same directories, and
  the shared headers from `include/` and `include/psxsdk/`. The copied headers
  do not all carry the licence marker of the C files. Any future publication
  of private source must review that provenance file by file.
- A private helper compiles every reference file with this project's
  toolchain, masks the relocated instruction fields and searches the image.
  179 inventoried library functions and 36 that the inventory had missed
  match a reference function in every fixed bit. 84 reference files match as
  whole objects, most of them one-function BIOS call stubs. 130 more
  functions have a close candidate. The relocations of the matches also give
  the address of 133 SDK symbols, with no conflict.
- The SDK in this executable is close to the reference but not the same
  version: two version strings differ. Files that match only in part need
  adapting.
- 15 reference files with no writable data of their own were built unchanged
  as units and are exact: 22 functions. Real SDK names come with them.
- **Correction to the eleventh group.** No library function needed a newer
  assembler. Every match was found with the project's assembler setting. What
  the library needed was the division expansion, now a build option. The
  per-unit assembler setting is dropped as a plan.
- The function inventory undercounts library code: 36 functions reached only
  through pointers have no entry. The percentages above use the inventory as
  it is.

Build tool:

- Division expansion, include directories, read-only data that ends short of
  a word, and a float guard that ignores literals are in. One matched unit
  needed the padding rule.
- Ready on a branch, not merged: units that own initialised data and declare
  their bss, assembly units counted apart from C, and a check that rejects
  inline assembly in C units. A trial showed the reference files with data
  still fail there for a reason not yet examined: the native compiler crashes
  on three of them under the build's preprocessing, though it compiles them
  in the identification helper, which passes the reference's own defines.

Lessons:

- The banking helper parked a whole batch of matched functions when the build
  stopped on a configuration error and left no objects. Nothing was lost, the
  candidates were restored from the parked copies. The helper now stops on
  configuration and environment errors.
- Two helpers dropped a unit's read-only data line when they removed or split
  the unit, which broke the next unit's table. Fixed.
- A third function needed a second symbol for one address inside the shared
  state block to get its own base register.
- One agent redid a function that the parked retry had already recovered,
  because the round was planned from a tree older than the retry. Plan rounds
  from the newest tree.

## Nineteenth group: data and assembly owners, the rest of the identified SDK

Counts at this snapshot. No round is in progress.

- 1,331 functions exact. 1,289 come from C, 179,396 bytes. 42 come from
  assembly, 672 bytes. Units own 236 bytes of read-only data in seven ranges
  and 4,356 bytes of initialised data in five. 429,740 payload bytes stay
  raw. About 55 percent of the inventoried resident code bytes: 179,280 of
  328,096, counting only functions the inventory lists, at its sizes.
- 57 functions are new: 15 from C and the 42 from assembly. 34 of them came
  out of the parked pool, where agents had tried the stubs and parts of the
  interrupt file in C. 5 came from the unattempted ones. 18 have no
  inventory entry.
- Parked: 253 functions, about 77 KB (210 game, 43 library). Unattempted: 86
  functions, about 72 KB (6 game functions of 1,700 bytes and more, 80
  library). The permutation search has 167 parked functions left to try.
- By area: 1,087 game functions and 244 library functions are exact.

Build tool, merged:

- Units own initialised data and declare their bss. Units can be written in
  assembly and are counted apart from C. A C unit with inline assembly is
  rejected.
- **A unit owns the symbols its object defines.** The review found a false
  pass: a unit's bss declared at a wrong address still built exact when the
  symbol file assigned the unit's own variables their old addresses. The
  link bound them as absolute and nothing compared the declared range with
  where the variables really were. The build now rejects any symbol-file
  name that a unit object defines as a global or weak symbol, and checks in
  the linked image that each such symbol sits in its unit's range. A second
  name for the same address stays allowed.
- The instruction diff reads the data and bss symbols of sibling units from
  their objects, so it links a unit that uses another unit's variables
  without a whole-image link.

SDK import:

- Five reference files with data of their own are in as C units: four from
  the interrupt and vertical-sync code of `libetc` and one from `libsnd`. 27
  functions. 12 of those were already exact as units written by agents and
  changed owner to the reference source, which brings the real SDK names.
- One of the five is the first adapted file. Its code equals the reference.
  Its version string names an older revision of the file, 1.71 against 1.73
  in the reference, so the private copy carries this executable's string.
  That replacement is the only change. The other 19 imported C files are
  built unchanged. 20 files and 49 functions in total.
- 42 stubs are assembly units of four instructions, 16 bytes each: 40 jump
  into the BIOS through its three call tables, 2 are system calls. In the
  reference each is a C file holding one macro that expands to inline
  assembly. The assembly source here is generated from the macro's
  arguments. The function inventory lists the BIOS stubs as 12 bytes: the
  closing `nop` is part of the stub in the reference macro and the unit owns
  it.
- Every reference file that was identified as a whole object is now in the
  build. What remains of the library needs adapted source.
- **Correction to the eighteenth group.** The files with data did not fail
  for a compiler crash here. With the pinned compiler as the default, four
  of the five built and matched at once. The fifth failed twice for other
  reasons: the import had not copied a header it includes from another
  library's directory, and then its version string differed. The crash of
  the native macOS compiler on these files was not examined again.

Second working machine:

- Since 2026-10-03 the work also runs on the Linux host that holds the
  pinned compiler. There it is the default compiler, with no remote step and
  no float restriction. Both machines produce the same image.
- The build-tool suite adds its two float cases only where the default
  compiler is marked `no_float`. A full run is therefore two cases smaller
  on the Linux host. State which machine a case count comes from.

Lessons:

- A count of test cases is a property of the configuration, not only of the
  code. The reviewer's count and the local one differed by the two float
  cases.
- An override in the symbol file is silent for data just as it is for
  functions. The earlier lesson covered functions only.

## Twentieth group: parts of partly identified SDK files

Counts at this snapshot. No round is in progress.

- 1,357 functions exact. 1,315 come from C, 187,008 bytes, and 42 from
  assembly, 672 bytes. Units own 392 bytes of read-only data in nine ranges
  and 4,356 bytes of initialised data in five. 421,972 payload bytes stay
  raw. About 57 percent of the inventoried resident code bytes: 186,440 of
  328,096, counted as before.
- 26 functions are new, 7,612 bytes. 3 came out of the parked pool and 13
  from the unattempted ones. 10 have no inventory entry.
- Parked: 250 functions, about 77 KB (210 game, 40 library). Unattempted: 73
  functions, about 65 KB (6 game functions of 1,700 bytes and more, 67
  library). The permutation search has 164 parked functions left to try.
- By area: 1,087 game functions and 270 library functions are exact. 84 of
  the library functions are built from reference C: 49 in 20 whole files and
  35 in 13 parts.

What a part is:

- A reference file that the identification found only in part still holds
  functions that compile to the original bytes as they are. A run of such
  functions, neighbours in the reference file and gap-free in the image,
  becomes one unit. 13 units from four files of `libgpu`, `libsnd` and
  `libspu`, 35 functions. 9 of them were already exact as units written by
  agents and changed owner.
- The source of a part is the reference file with every function outside the
  run reduced to its prototype and the lines that pull in assembly removed.
  The function bodies are unchanged. A function that was `static` and now
  lives outside the part gets a global prototype.
- The address of a part's read-only data, and of every name it uses that was
  not located before, is read from the original code at the object's
  relocation sites. A wrong address cannot pass, because the build compares
  the linked bytes.
- A part is a stage, not the goal. When a file's remaining functions match,
  its parts are replaced by one unit for the whole file.

What the parts showed about this SDK version:

- Two functions that share one file in the reference are two separate
  objects here: another file's object sits between them in the image. Each
  came in as its own part with no change to the code.
- In one file the image holds a function, between two of the file's
  functions, that the reference file does not define. In another, one
  function is kept as assembly by the reference itself. Both stay raw.
- One function's read-only data is not one range in the image: 16 bytes of
  other data sit between its items. It stays raw until the file is whole. Of
  the eight other functions of its run, five came in one by one and three
  were exact already.
- Six partly identified files have writable data. A part cannot own a file's
  data, because several parts of one file would each define it. They are not
  handled yet.

Lessons:

- The symbol rule made its first catch on real input. The import re-added the
  address of a variable that a unit from the previous import defines, and the
  build refused it. The helper now treats the symbols of every imported file
  as defined, not only those of the files it is importing in that run.
- Function definitions on one line broke the first split of a reference
  file. The split now handles them.

## Twenty-first group: the first adapted header

Counts at this snapshot. No round is in progress.

- 1,385 functions exact. 1,343 come from C, 200,332 bytes, and 42 from
  assembly, 672 bytes. Read-only data and initialised data are unchanged at
  392 and 4,356 bytes. 408,648 payload bytes stay raw. About 61 percent of
  the inventoried resident code bytes: 198,744 of 328,096, counted as before.
- 28 functions are new, 13,324 bytes. 5 came out of the parked pool and 16
  from the unattempted ones. 7 have no inventory entry.
- Parked: 245 functions, about 76 KB (210 game, 35 library). Unattempted: 57
  functions, about 53 KB (6 game functions of 1,700 bytes and more, 51
  library). The permutation search has 159 parked functions left to try.
- By area: 1,087 game functions and 298 library functions are exact. 123 of
  the library functions are built from reference C: 49 in 20 whole files and
  74 in 17 parts from five files.

What was found:

- The largest partly identified file is the voice manager of the sound
  library: 50 functions in the reference, most of them close to the image and
  not equal. A word-by-word comparison of one such function with the image,
  ignoring the fields that relocations fill, showed three differing
  instructions, all the same constant: an element size of 48 bytes here
  against 52 in the reference. Other functions multiply an index by 48 where
  the reference multiplies by 52.
- Mapping every data address the code uses against the reference's offsets
  placed the difference: the first 8 bytes of the per-voice struct agree,
  everything after is 4 bytes lower, and what follows the array of 24 voices
  is 96 bytes lower. This SDK version's struct has no fields at offsets 8 to
  11, where the reference has three.
- The private copy of that header drops the three fields. It is the first
  adapted header. With it, 39 functions of the file are exact as the
  reference wrote them: the function bodies are unchanged, only the struct
  they compile against differs. 11 of them were already exact as units
  written by agents and changed owner. They came in as four more parts.
- Every other unit that includes the header was rebuilt against the adapted
  copy and stayed exact.
- Seven functions of the file are still not exact. Three of them use the
  missing fields in the reference, so their code really differs here. The
  rest differ in a few instructions or were not compared yet.

How the tools changed, all private:

- Adapted files live in their own folder under the reference's relative
  paths. The identification and the import read the reference with those
  files laid over it, so an adapted header reaches every file that includes
  it. Two files are adapted in that folder so far: the header, and the voice
  manager source, which gained an external variant of its data and had the
  statements that use the missing fields removed or stubbed, all in
  functions that are not exact yet. The version string of the interrupt file
  is still a listed replacement.
- A part cannot own its file's data. The adapted source offers its data as
  external declarations under a macro that part sources define. The
  addresses come from the original code, as for any other name.
- The identification now also places a function by its neighbours. Between
  two exact functions whose distance in the image equals their distance in
  the object, a function in between is exact if it matches at the same
  relative place. This found eight functions that are too short or too
  common to be located alone, such as empty functions and twins with
  identical code.
- A checker compiles one adapted file and compares each of its functions
  with the image in about a second. It is the working loop for adapting a
  file and decides nothing.

Lesson:

- Look for a regular difference before working on single functions. Most of
  the near misses in this file were one struct. Finding it took two small
  comparisons and was worth 13 KB of code.

## Twenty-second group: reference files adapted to this SDK version

Counts at this snapshot. No round is in progress.

- 1,419 functions exact. 1,377 come from C, 214,556 bytes, and 42 from
  assembly, 672 bytes. Units own 1,456 bytes of read-only data in 15 ranges
  and 5,084 bytes of initialised data in seven. 392,632 payload bytes stay
  raw. About 65 percent of the inventoried resident code bytes: 212,160 of
  328,096, counted as before.
- 34 functions are new, 14,224 bytes. 8 came out of the parked pool and 17
  from the unattempted ones. 9 have no inventory entry.
- Parked: 237 functions, about 75 KB (210 game, 27 library). Unattempted: 40
  functions, about 41 KB (6 game functions of 1,700 bytes and more, 34
  library). The permutation search has 151 parked functions left to try.
- By area: 1,087 game functions and 332 library functions are exact. 197 of
  the library functions are built from reference C: 66 in 27 whole files and
  131 in 22 parts from nine files. 164 of the 197 have the reference's body
  unchanged. 33 are adapted, two of which are one-line functions the
  reference file does not have. 40 functions that agents had matched from
  the disassembly changed owner to reference source in this step.

How the work was done:

- Six agents, one library area each, edited only adapted copies of that
  library's files. Their loop was the checker: it compiles one file and
  compares each function with the image in about a second. An agent's
  result counted for nothing by itself. Afterwards the identification and
  the import ran again over all adapted files and the build decided.
- 22 files are adapted now, three of them headers. The interrupt file's
  version string is still a listed replacement.

What this SDK version does differently, as far as the exact code shows:

- Sound library. The tick settings are separate variables, not one struct.
  The internal init function takes a flag, and the two public init entry
  points are one-line wrappers in the same file. Note-on has no mute check,
  and note-on and the control change handler scale volume differently. The
  voice key-on does no per-score volume scaling.
- CD library. The control functions wait for the drive lid through a
  function the reference does not have, instead of retrying a no-op
  command. Timeouts are half as long. The interrupt state has one more
  byte, used to defer the acknowledge. One message string differs.
- Graphics library. The library state is one struct of 128 bytes where the
  reference has separate variables. The drawing area limits come from two
  small tables indexed by the GPU type. One message string is spelled
  differently.
- SPU library. The hardware init sets the voice registers through an
  internal function that the reference does not have.
- Three image functions that the identification had taken for one reference
  function are something else: each delivers an event with fixed arguments.

Open, with what is known:

- **One object looks assembled by another assembler.** The root counter
  file of the system library shows small constants loaded with `addiu`, a
  three-instruction table lookup, and stores in the delay slot of the
  return. The project's assembler setting does not produce these. Running
  the conversion by hand with a newer setting gave the lookup form and the
  delay-slot store. This reopens the per-unit assembler setting that the
  eighteenth group dropped. It needs a tool change and is not decided.
- **Struct copies through a call.** Four graphics functions copy a struct
  by calling the BIOS copy routine. The compiler expands a `memcpy` of
  constant size in place. Not tried yet: a plain struct assignment, which
  this compiler may turn into a call. A compiler flag for those units is the
  other way and needs a decision.
- **Loads that only a volatile field reproduces.** Several graphics
  functions load the GPU type through a pointer register and the limits
  with a sign extension. The agent found only a `volatile` field gives that
  form, and it breaks other functions. Not used.
- One SPU function is one instruction short: the image has a `nop` after a
  load that the conversion step does not emit. The cause is not known.
- The functions the reference does not have were not written: four in the
  CD library, one in the SPU library, one in the graphics library, and one
  the reference keeps as assembly in the sound library.
- In the CD library's main file, several exact functions print through one
  inline function, and the image holds that message once. A part would hold
  its own copy, so those functions stay raw until the file is whole.

Honesty notes:

- An adapted function says what the image does, starting from the
  reference. It is not the reference's function any more and is counted
  apart from the unchanged ones.
- Names for things the reference does not have are placeholders chosen by
  the agents and marked in the source: the separate tick variables, the
  graphics state struct, the lid-wait function, a few others. They are not
  recovered names.
- Agents flagged edits whose only effect is on register use or frame size.
  All of them are in functions that are not exact and are in no unit. They
  are to be removed or justified before such a function is accepted.
- **A function can be exact in code and wrong in data.** The comparison
  skips the instruction fields that hold addresses, so it cannot see a
  string that differs. Two message strings differed. The checker now also
  compares the strings a function refers to. The build compared them all
  along, which is why those functions had been left out.

How the tools changed, all private:

- A part keeps the body of an inline function that lies outside its run, as
  `extern inline`, so that the run's functions still expand it in place and
  the unit emits no copy. Without that, three functions were 4 bytes short.
- When the addresses in the code do not settle where a part's read-only
  data lives, the import searches the image for the bytes.
- Every import now removes all parts and generates them again, so that a
  run that grew or a file that became whole replaces what was there.

Lessons:

- Never tidy a folder that running agents are working in. A cleanup that
  removed unmodified copies deleted five files two agents had just copied
  to start on. They were restored within a minute and held no edits.
- A stale identification record gives a wrong unit: after editing an adapted
  file, the identification has to run again before the import.

## Twenty-third group: large game functions, fourth permutation sweep, second SDK pass

Counts at this snapshot. The fourth permutation sweep is still running: it
had tried 62 of 151 parked functions, and its 16 matches up to then are
included. Nothing else is in progress.

- 1,441 functions exact. 1,399 come from C, 223,596 bytes, and 42 from
  assembly, 672 bytes. Units own 1,480 bytes of read-only data in 16 ranges
  and 5,084 bytes of initialised data in seven. 383,568 payload bytes stay
  raw. About 67 percent of the inventoried resident code bytes: 221,200 of
  328,096, counted as before.
- 22 functions are new, 9,040 bytes: 18 game functions and 4 library
  functions. All 22 are in the inventory.
- Parked: 223 functions, about 79 KB (198 game, 25 library). Unattempted: 32
  functions, about 28 KB, all library. The pools moved like this: 16 parked
  game functions matched in the sweep, 2 parked library functions matched,
  2 unattempted game and 2 unattempted library functions matched, and the 4
  large game functions that did not match went from unattempted to parked.
- By area: 1,105 game functions and 336 library functions are exact. 206 of
  the library functions are built from reference C: 70 in 28 whole files and
  136 in 24 parts from nine files.

Large game functions:

- The six game functions of 1,700 to 2,436 bytes went to one agent each. Two
  are exact, 1,944 and 2,420 bytes. With the earlier rounds that is 5 of 22
  at 1,000 bytes and more.
- The four misses: one has the right size and differs in 46 instruction
  slots by register choice. The others are 16, 24 and 32 bytes short, with
  a merged division, a comparison the compiler folds away, and tails the
  compiler merges or keeps apart. All four candidates are parked.
- One agent noted that the rigid order of loads and stores around a few
  scratch globals in its function looks like `volatile` access. It did not
  use it. This is the same question as in the graphics library and is not
  decided.

Permutation search:

- The search runs on this machine now. The fourth sweep covers the parked
  functions the earlier sweeps had not tried, four minutes each, in three
  parallel runs.
- All 16 matches it had found at this snapshot passed the retry of the
  parked pool and the build. That makes 36 functions whose matching form was
  found by the search, out of 169 tried so far.
- **Every machine-found function now says so in its source.** Until now only
  one of the earlier 20 carried the comment. The search renames nothing and
  inserts no dummy code, but it adds temporaries and reshapes expressions,
  and a reader should know that the form was not written by hand. The saved
  form from before the search is kept privately for each of them.

Second pass over the adapted SDK files:

- Agents were now allowed to write functions the reference does not have.
  Banked from that pass: two rewritten functions and one new function in the
  graphics library, and one more whole file in the CD library that the
  identification had skipped for a guard that this machine does not need.
- Written but not banked: three of four missing functions of the CD
  library's main file are exact by the checker, yet none of that file's
  functions with strings can be owned before the file is whole, and two of
  its functions still differ by register choice. The large sound function
  the reference keeps as assembly is written and 24 bytes short in its
  stack frame. The SPU function the reference lacks is 8 bytes too long.
- One function is exact only with `volatile` on a variable the reference
  does not have. It is held back with the other `volatile` cases.
- Tested: a struct assignment and a `memcpy` of any form are both expanded
  in place by this compiler. Only `-fno-builtin` gives the call that four
  graphics functions have in the image. Setting that flag for those units is
  a decision that is still open.

**A function can be exact in code and wrong in which variable it touches.**

- The comparison skips the instruction fields that relocations fill. Two
  stores of the same shape to two different variables then look alike. One
  adapted function zeroed two queue indices in the wrong order and passed
  the checker. The import caught it only by accident, because the two names
  came out with two addresses each and stayed unresolved.
- The checker now requires that every name stands for one address across a
  file, and reports where the code implies two. It also reports when a
  file's own data is not laid out as in the image, which means the file
  cannot become one whole unit yet.
- The build was never fooled: it compares linked bytes, addresses included.

Tool changes, all private:

- Functions that are `static` in the reference are global in a part,
  because another part of the same file may call them. Without that, a part
  could not link against a function that had become exact in a sibling
  part, and three parts that had been in the build dropped out of a trial
  import.
- The address pairing in the identification now follows the base register,
  so that two interleaved address computations are not mixed up.
- The identification no longer refuses floating-point source on a machine
  whose compiler handles it. One more reference file built and matched whole.

Lesson:

- Regenerating all parts on every import is right, and it means an import
  can lose functions as well as gain them. Compare the set of owned
  addresses with the promoted tree before promoting. A trial import fell
  from 1,419 to 1,402 before the cause was fixed.

## Twenty-fourth group: end of the fourth sweep, assembly units for assembly code

Counts at this snapshot. No round is in progress.

- 1,466 functions exact. 1,413 come from C, 225,816 bytes. 53 come from
  assembly, 1,040 bytes. Read-only data and initialised data are unchanged at
  1,480 and 5,084 bytes. 380,980 payload bytes stay raw. About 68 percent of
  the inventoried resident code bytes: 223,768 of 328,096, counted as before.
- 25 functions are new, 2,588 bytes: 14 from C, 2,220 bytes, and 11 from
  assembly, 368 bytes. 14 are game functions and 11 are library functions.
  All 25 came out of the parked pool.
- Parked: 198 functions, about 76 KB (184 game, 14 library). Unattempted: 32
  functions, about 28 KB, all library.
- By area: 1,119 game functions and 347 library functions are exact.
- **Scope of "every game function has been attempted".** It means the game
  functions of the resident function inventory. It says nothing about the
  overlays or about a whole-game inventory, which do not exist yet.

Fourth permutation sweep, finished:

- 151 parked functions tried, four minutes each. 30 matched and all 30
  passed the retry of the parked pool and the build. The candidates of 5 did
  not compile and were skipped.
- Over all sweeps: 258 functions tried, 50 matched. Every one of the 50 says
  in its source that its form was machine-found.
- Every parked function has now been through the search once. More of the
  same search on the same candidates is not expected to give much. What is
  left needs a different candidate first.

Assembly units for code that was assembly:

- 11 functions that C cannot produce are assembly units now, written from
  the disassembly one instruction per line: the program entry routine, a
  function that returns the global pointer, four calls to a debugging host
  through the `break` instruction, and five BIOS call stubs that the SDK
  reference has no file for.
- The entry routine clears the bss, sets the stack, frame and global
  pointers, sets up the heap and calls the main function. An agent had parked
  it as not matchable from C.
- The four words that follow the entry routine in the image are a table it
  reads. They are data inside the code area and belong to no function. They
  stay raw.
- These units keep `func_<address>` names. What each one does is in a
  comment. The BIOS stubs name the table and the function number.
- **Rule applied.** An assembly unit is for code whose original source was
  assembly. The evidence here is the instructions themselves: `break`, a
  jump through a register loaded with a BIOS table address, reading the
  global pointer, setting the stack pointer. None of this is game logic.
  Writing a game function as assembly because it resists C is not the same
  thing and has not been done.

Lesson:

- The retry helper took a parked candidate for a function that the base
  already owned as an assembly unit and stopped on the duplicate name. It
  now skips functions the base owns.

## Twenty-fifth group: second attempt on the parked pool

Counts at this snapshot. The fifth permutation sweep is still running: it
had tried about half of its 63 functions, and its 2 matches up to then are
included. Nothing else is in progress.

- 1,491 functions exact. 1,438 come from C, 231,308 bytes, and 53 from
  assembly, 1,040 bytes. Read-only data and initialised data are unchanged at
  1,480 and 5,084 bytes. 375,488 payload bytes stay raw. About 70 percent of
  the inventoried resident code bytes: 229,236 of 328,096, counted as before.
- 25 functions are new, 5,492 bytes, all game functions of the resident
  inventory, all out of the parked pool. One of them is 24 bytes longer than
  the inventory lists it: the epilogue after an endless loop.
- Parked: 173 functions, about 71 KB (159 game, 14 library). Unattempted: 32
  functions, about 28 KB, all library.
- By area: 1,144 game functions and 347 library functions are exact.

Where the 25 came from:

- 20 from agents that retried parked functions. Each agent got five
  functions and started from the earlier candidate, not from nothing: build,
  read the diff, name the kind of residual, then try what the candidate had
  not tried. Two waves of eight agents, 80 functions. The first wave took
  the candidates the permutation search had scored as middling: 13 of 40.
  The second took the far ones and those never scored: 7 of 40, one of them
  only short of the epilogue that the retry helper adds.
- 3 by hand. Two were byte-exact results of the search that it had refused
  to write back because they carried junk: a self-assignment in one, an
  unused `volatile` variable in the other. Without the junk the first is
  exact as it stands. The second needs an unused local array instead, the
  stand-in already used in four units for a stack frame the original
  reserves and never uses. A script then tried that stand-in on all 198
  parked candidates: one more became exact, two came closer.
- 2 from the fifth permutation sweep so far.

What made retries work, from the agents' reports:

- A local pointer to a player object or to the state block, where the
  original keeps the address in a saved register. The second player written
  as the first plus one, or the first as the second minus one.
- A shared tail split into two separate calls, or early returns in place of
  a shared state variable. The opposite also occurred: two branches merged
  into one condition.
- A `goto` to keep a test or a tail where the original has it. A retry loop
  written as a `for` with a `goto` back, not as nested `do/while`.
- A test routed through a local, so that the compiler emits the mask and the
  compare as two instructions.

Residual kinds that came back again and stay unsolved:

- **High byte through two shifts.** Four more functions take the
  high byte of a loaded halfword with a shift left by 16 and an arithmetic
  shift right by 24, after copying the value to a second register. Every
  form tried folds to a shift right by 8: signed, unsigned and int locals,
  explicit shifts, a double read, a copy through another type. Writing the
  two shifts out did match one function where the value is a product, so
  the form depends on what is shifted.
- **Parameters copied out of the argument registers at entry.** Several
  originals move their parameters into other registers first and then use
  the argument registers as temporaries. One guess was that these are
  bodies of inline functions expanded into a thin caller. A test on one
  function did not confirm it: the build was no closer. The cause is not
  known.
- Loads the original schedules after a computation where the build hoists
  them, two saved registers swapped that no declaration order moves, and a
  frame that is larger above the spill slots, where an unused local lands
  below them.

Judgment calls taken at the top level:

- One exact function picks the left or the right player by arithmetic on
  the address of one field of the left player, because that is how the
  original derives both addresses, and no selection through the fields
  matched. It is written plainly with comments and flagged here.
- Unused local arrays as a frame stand-in: the earlier notes count four
  units, and two were added in this step, with one fixed comment that says
  it is a stand-in and not an explanation. This is the only dummy construct
  that is accepted.
- The two search results cleaned by hand count as machine-found and say so.
  That makes 54 marked functions.

Tool changes, all private:

- A sweep now writes a clearly closer variant back as the parked candidate
  when it has no junk, so that the next sweep continues from it. Before,
  every sweep started from the same candidate.
- A helper sets up a retry batch from parked candidates, and there is a
  prompt for retries that lists what has worked.

Mistake:

- The script that tried the unused local scored each variant with a helper
  that rebuilds a function's search folder. That deleted the best
  non-matching variants the fourth sweep had left for the parked functions.
  Nothing that counts was lost: matches are written back at once and were
  already banked. The fifth sweep regenerates what is useful. The lesson is
  to check what a helper deletes before running it over the whole pool.

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
