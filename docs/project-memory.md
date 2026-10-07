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

## Twenty-sixth group: third retry wave, end of the fifth sweep, 1,500 functions

Counts at this snapshot. No round is in progress.

- 1,500 functions exact. 1,447 come from C, 233,184 bytes, and 53 from
  assembly, 1,040 bytes. Read-only data and initialised data are unchanged at
  1,480 and 5,084 bytes. 373,612 payload bytes stay raw. About 70 percent of
  the inventoried resident code bytes: 231,112 of 328,096, counted as before.
- 9 functions are new, 1,876 bytes, all game functions of the resident
  inventory, all out of the parked pool.
- Parked: 164 functions, about 69 KB (150 game, 14 library). Unattempted: 32
  functions, about 28 KB, all library.
- By area: 1,153 game functions and 347 library functions are exact. That
  is a count by address, below or above the start of the library.
- Against the inventory: 1,152 of its 1,302 game functions are exact, with
  173,420 of their 240,200 bytes, about 72 percent. The other 150 are the
  parked game functions. The by-area count has one more because one exact
  function in the game area, 8 bytes right before the object reset routine,
  has no inventory entry. Keep the two counts apart: a by-area total is not
  a numerator over the inventory.

Where the 9 came from:

- 6 from a third wave of retries, eight agents on the last 38 far
  candidates. Over the three waves agents retried 118 parked game functions
  and matched 26: 13 of the 40 middling ones, 7 of the first 40 far ones, 6
  of the last 38.
- 3 from the rest of the fifth permutation sweep. The sweep tried the 63
  closest candidates for six minutes each and matched 5. For 32 more it
  found a clearly closer variant and kept it as the new candidate.
- 57 functions carry the machine-found mark now.

Cleanup asked for by the review:

- 29 source files of exact units still held notes that said "residual" or
  "not exact". They dated from before the match, or described a function
  that had since been cut out of the unit. Each such note now starts with a
  label that says it is historical. A helper applies the label before every
  promotion.

Lessons:

- **A merge can carry unverified additions.** A batch directory that kept
  one exact function also holds the fields and symbols its agent added for
  the functions that did not match. The merge took them all: 13 fields and
  25 symbols that nothing exact uses. They were removed again before the
  build, and only the three fields the exact unit needs stayed. They are
  kept as notes next to the parked candidates. The earlier rule covered
  only batches that matched nothing.
- The first attempt to remove them compared the field file line by line and
  deleted fields that the merge had merely reformatted. The build caught it
  at once. Compare the parsed model, not the text.
- A parked candidate can depend on a field its batch added and that was
  never merged. It then does not compile against the tree, and the
  permutation search skips it. The notes above are what a later retry needs.

## Twenty-seventh group: the high-byte form, sixth sweep

Counts at this snapshot. No round is in progress.

- 1,515 functions exact. 1,462 come from C, 236,724 bytes, and 53 from
  assembly, 1,040 bytes. Read-only data and initialised data are unchanged at
  1,480 and 5,084 bytes. 370,072 payload bytes stay raw. About 72 percent of
  the inventoried resident code bytes: 234,652 of 328,096, counted as before.
- 15 functions are new, 3,540 bytes, all game functions of the resident
  inventory, all out of the parked pool.
- Parked: 149 functions, about 66 KB (135 game, 14 library). Unattempted: 32
  functions, about 28 KB, all library.
- Against the inventory: 1,167 of its 1,302 game functions are exact, with
  176,960 of their 240,200 bytes, about 74 percent. The other 135 are parked.
  By area the count is 1,168 game functions and 347 library functions; the
  one more is the 8-byte function the inventory does not list.

The high byte taken with two shifts:

- Sixteen game functions in the image take the high byte of a loaded
  halfword with a shift left by 16 and a shift right by 24. Half of them
  were exact already and half were parked with that as their residual. All
  but three sit within 12 KB of each other.
- Four enumerations of the source form on one small parked function, about
  9,000 variants compiled and compared in about a minute, did not find it.
  Neither did 27 sets of compiler flags.
- The form was in the tree all along, in five units that the permutation
  search had matched: copy the halfword into a signed 16-bit local, shift
  that right by 8 into an unsigned 16-bit local, and store that local. The
  unsigned 16-bit temporary is what keeps the two shifts. With any other
  type the compiler folds them into one shift right by 8. The first
  enumeration had tried every type for that temporary except this one.
- With that form all eight parked functions produce the two shifts. One
  became exact at once. Three more were matched by the search after it, and
  those showed the second half: where the original has a register copy
  between the load and the shifts, the source reused one local for a
  second, unrelated value later in the function. Three functions still lack
  that copy and one differs in two slots.
- A local union of a word and its two bytes was also tested. It does make
  the compiler reserve an unused stack frame, which no other source form had
  done by itself, but the frame has another size and the extraction is not
  the two shifts. Not the answer, and kept as a lead for the unused frames.

Where the 15 came from:

- 11 from the sixth permutation sweep: all parked game candidates except
  the eight above, five minutes each. 142 tried, 56 more left with a closer
  candidate, 6 whose candidate does not compile against the tree, 2 that
  were byte-exact but carried junk and were not written back.
- 4 of the eight functions with the two shifts: 1 by an agent with the
  form, 3 by a longer search run started from the candidates with the form.

Marks:

- 71 functions say that their form was found by the search. One more says
  that it was finished by hand from a candidate the search had reshaped.
  Since a sweep now writes closer candidates back, a saved earlier form no
  longer proves that the search found the final one. The mark is given only
  when a sweep reports the match.

Lessons:

- **Read the exact units before enumerating.** The answer to a residual
  that agents had failed on about ten times was already in five matched
  files. A search over the image for the instruction pattern, then a look
  at which of those functions are exact, found it in minutes.
- A fast enumeration over one function is cheap to set up with the search's
  own compile script: thousands of variants a minute. It only finds what
  its grid contains.

## Twenty-eighth group: library functions that were exact but not imported

Counts at this snapshot. A seventh permutation sweep over the parked game
functions is running and is not part of these counts.

- 1,545 functions exact. 1,492 come from C, 243,804 bytes, and 53 from
  assembly, 1,040 bytes. Read-only data owned by units is 1,808 bytes in 23
  ranges; initialised data is unchanged at 5,084 bytes. 362,664 payload
  bytes stay raw. About 74 percent of the inventoried resident code bytes:
  241,408 of 328,096, counted as before.
- 30 functions are new, 7,080 bytes, all library. 14 of them are in the
  resident inventory, 6,756 bytes. The other 16 are small library functions
  that the inventory does not list.
- Against the inventory: 315 of its 347 library functions are exact, 64,448
  of 87,896 bytes. Game functions are unchanged: 1,167 of 1,302. By area the
  count is 1,168 game functions and 377 library functions.
- Parked: 144 functions, about 64 KB (135 game, 9 library). Unattempted: 23
  functions, about 22 KB, all library.
- 245 C functions now sit in SDK units, 39 more than before: the 30 new ones
  and 9 that address-named units owned and that now carry their SDK names.

What was in the way:

- Thirteen library functions compiled exact for several rounds and still
  counted as unattempted or parked. The importer skipped them each time
  with the same reason: their read-only data could not be placed.
- The compiler keeps one copy of identical string literals in a file and
  emits an inline function's strings where the function is defined, used or
  not. A function built alone, as a part of a file that is not whole yet,
  therefore gets such a string next to its own strings, where the image
  does not have it. Three message strings did this: two that inline helpers
  of the CD file carry, and one that two functions of the SPU file share.
- An adapted file now names such a string as an external array when it is
  built as a part, exactly as it already does for the file's data. The
  code is the same. The string's bytes stay raw until the file is whole.
- The primitive file of the GPU library was exact in every function and
  still not whole: in this SDK version one setter sits after the line
  setters, and one 28-byte function follows it that the reconstruction does
  not have. With the order fixed and that function written, the file is
  one unit with all 38 functions and its strings.

A wrong reference that the comparison could not see:

- One CD function was reported exact by the file checker and failed in the
  build by one word. Its source installed the interrupt callback as the
  data-ready callback. The image installs another function there, 364
  bytes, absent from the reconstruction: the callback of a running read. It
  takes one sector per interrupt, restarts the read after an error and ends
  it. It is written now and exact; its name is not original.
- The checker masks the address fields of instructions, so a reference to
  the wrong function of the same file looked exact. It now checks that
  every reference to a function of the file leads to where that function
  was placed. Control: with the wrong callback put back, it objects.
- No published count was wrong: the function was never owned before this
  group. The build compares linked bytes and refused it.

Importer:

- A run of several functions that fails is now split and each function is
  judged alone, instead of the whole run being left out.
- An inline function that the image holds as a function of its own, because
  the file takes its address, is emitted as a plain function when it is a
  part by itself.

Still open in the library, 32 inventoried functions:

- CD: two functions. GPU system file: 13, eight unattempted and five
  parked, several of them behind the two open questions about volatile fields
  and struct copies through a call. Sound: five. SPU: two. Counter file:
  three, behind the assembler question.
- Seven have no source yet: three in the GPU area, one of them 4,480 bytes,
  three in the sound area and one parked.

Lessons:

- **A function that compiles exact and is not owned is a finding.** Read
  the importer's skip list after every import. The reason was printed for
  several rounds and nobody acted on it.
- A masked comparison proves the instructions, not which symbol an address
  field names. Names need their own check, also for functions.

## Twenty-ninth group: the CD file whole, a wave of one-function packages

Counts at this snapshot. The seventh permutation sweep over the parked game
functions is still running and is not part of these counts.

- 1,554 functions exact. 1,501 come from C, 254,744 bytes, and 53 from
  assembly, 1,040 bytes. Units own 2,264 bytes of read-only data in 18
  ranges and 5,900 bytes of initialised data in eight. 350,452 payload bytes
  stay raw. About 77 percent of the inventoried resident code bytes: 252,348
  of 328,096, counted as before.
- 9 functions are new, 10,940 bytes, all library and all in the inventory.
- Against the inventory: 324 of its 347 library functions are exact, 75,388
  of 87,896 bytes. Game functions are unchanged: 1,167 of 1,302. By area the
  count is 1,168 game functions and 386 library functions.
- Parked: 144 functions, about 64 KB (135 game, 9 library). Unattempted by
  the counting rule: 14 functions, about 11 KB, all library. Each of the 14
  has a candidate in an adapted library file that still differs; the rule
  calls a function parked only when its candidate sits in the parked pool.
- 253 C functions sit in SDK units.

Correction to the twenty-eighth group: it said that seven library functions
had no source and 25 had an adapted source that differed. Two of the 25 had
no source either. Their nearest match named two reference functions that are
exact elsewhere in the image; the two are variants for another file format
that the reconstruction does not have. Nine had no source, 23 had one.

The CD file, two functions and the file's data:

- The interrupt routine was one instruction too long. Its local buffer is
  `volatile` in the reconstruction and is not here: a plain load needs no
  separate zero extension.
- The read-wait routine had two saved registers swapped. Returning the
  result inside the loop instead of after it made it exact. The compiler
  weights each use of a value by loop depth when it hands out saved
  registers, so where the last read sits decides.
- Data layout of this SDK version: the read state table is initialised data
  and follows the interrupt state, two byte variables are in the other
  order, one table entry differs, and the init structure has five pointers
  and no version string. With that the file is one unit: 18 functions, 576
  bytes of read-only data, 816 bytes of data. The message strings that
  parts had to name are owned by the file again.
- Four variables of the file are common symbols. The original linker put
  them in the common area, which lies inside the zero-filled end of the
  executable. The build tool does not allow a unit bss range there, so the
  file names the four and the link places them by address.

A wave of ten packages, one function or one file each, seven exact:

- The two sound fade routines. This SDK version has them at 1,404 and 1,200
  bytes; the reconstruction has small later forms. Each does both
  directions here. The names follow the order in which their caller calls
  them, which is the only evidence for which is which.
- One SPU function: a value stored through a local, so that the compiler
  merges two stores.
- Four functions that had no source at all, written as address-named units:
  4,480, 684, 408 and 352 bytes. The largest is a decoder of drawing
  primitives with debug output, not the text formatter it was taken for.
  Roles are the agents' readings, not verified names.
- Each of the seven took an agent one to four minutes. Three of the four
  without a source were exact at the first build.
- Not exact: the sound data-entry routine (29 slots; the original frame is
  24 bytes larger with space that is never accessed), the two variants
  mentioned in the correction (written new; 40 and 23 slots), a voice
  allocation routine (the image reloads a global inside a loop that has no
  store and no call) and a volume routine (order of three saved registers).

Questions that need the owner, now four:

- New: the original assembler puts a delay instruction between a load into
  a register and a following load of a constant into the same register. The
  pinned assembler emulation does not. One SPU function differs only by
  that instruction. Tested in a scratch copy with a one-line rule: that
  function becomes exact and all 1,547 functions that were exact stay
  exact; the pattern occurs nowhere else in the tree. Upstream has no such
  rule. Taking it needs a way to carry a patch for the pinned tool.
- Unchanged: struct copies that the image does through a call, loads that
  only a volatile field reproduces, and the object that looks assembled by
  another assembler.

An observation about the raw count:

- The last 176,124 bytes of the payload, from 0x80183904, are zero. The
  uninitialised area of the program lies inside the file. Those bytes are
  counted as raw today, about half of all raw bytes. A unit cannot declare
  a bss range there. Counting that area by itself would need a change to
  the build tool and is not designed yet.

Tools and method, all private:

- A helper tries several complete bodies for one library function through
  the file checker and restores the file. Thousands of variants are not
  needed: five to twenty pointed ones decided every function above.
- A second brief for library agents lists the source forms that decided
  functions, and makes the shared header read-only while several agents
  work in one library.

Lessons:

- **Small packages with pointed context work.** One function per agent, the
  known residual and the neighbours to read in the prompt: seven of ten in
  minutes. The earlier per-library packages left exactly these functions.
- A near match that names a reference function which is exact elsewhere is
  another function. Check before counting it as an adapted source.
- A `volatile` in the reconstruction describes a later SDK version. Test
  the function without it before anything else when one instruction is
  extra after a load.
- When the roles of two saved registers are swapped, look at which uses sit
  inside which loop before permuting declarations.

## Thirtieth group: one-function retries on the parked game pool, seventh sweep

Counts at this snapshot. An eighth sweep over 32 parked candidates is
running and is not part of these counts.

- 1,582 functions exact. 1,529 come from C, 262,676 bytes, and 53 from
  assembly, 1,040 bytes. Read-only data and initialised data owned by units
  are unchanged at 2,264 and 5,900 bytes. 342,520 payload bytes stay raw.
  About 79 percent of the inventoried resident code bytes: 260,280 of
  328,096, counted as before.
- 28 functions are new, 7,932 bytes, all game functions of the resident
  inventory, all out of the parked pool.
- Against the inventory: 1,195 of its 1,302 game functions are exact,
  184,892 of 240,200 bytes, about 77 percent. The other 107 are parked.
  Library unchanged: 324 of 347. By area the count is 1,196 game functions
  and 386 library functions.
- Parked: 116 functions, about 57 KB (107 game, 9 library). Unattempted by
  the counting rule: 14 library functions, about 11 KB, as before.

Where the 28 came from:

- 20 from agents with one function each, 50 packages in five waves of ten,
  taken in order of the search's score for the candidate. By score: 12 of
  20 with a score up to 15, 6 of 16 from 20 to 60, 2 of 10 above 60, none
  of 4 whose candidate did not compile. An agent took between one and
  twenty-three minutes. Most of the 30 that did not match came back closer,
  several at one or two differing slots.
- 5 from the seventh permutation sweep: all 135 parked game candidates,
  five minutes each. 19 more left with a closer candidate.
- 3 that the search had found byte-exact and its own filter had rejected
  for a leftover unused variable, two of them already in the sixth sweep.
  Cleaned by hand: the variable removed, or replaced by the commented
  stand-in for an unused stack frame. One of the 20 agent results is a
  sibling of these, written in the same form.

Marks in the sources:

- 74 functions say that their form was found by the search, 5 more that it
  was found by the search and cleaned by hand, 11 that they were finished
  by hand from a candidate the search had reshaped, and 1 that it was
  written in the form of a sibling the search had found. A function carries
  the first mark only when a sweep log reports the match.

Source forms that decided functions, each the only change needed:

- `mask &= flags; if (mask)` instead of `if (mask & flags)`.
- Separate paths that each end in their own call, instead of one shared
  index local and one call.
- Return type `int` with `if (a && b) return 1; return 0;`.
- A callee prototype with one more parameter, so that a loaded value lands
  in the argument register.
- A read placed inside the condition with a comma expression, to fix the
  order of two loads.
- A load into a wider local copied into a narrower one, where the original
  has a register copy that the compiler otherwise merges away.
- The base written first in an address sum, where one `addu` had its
  operands the other way round.
- The operands of one `|` chain reordered.
- A global declared as an array and read as its first element. The
  compiler orders such a load after stores through an object pointer and
  does not order a scalar global that way. The array declaration is a
  compatible reconstruction: it reproduces the load schedule. It does not
  establish the original declaration or data layout. Inferred hypothesis,
  unproven: the original reads that global as an array element or a struct
  member.

Evidence for the open question about `volatile`, a fifth case and the
first in game code:

- One 72-byte function is exact when its counter is declared `volatile`
  and incremented with a plain `++`: the original reloads the counter after
  the store into a register that nothing reads. No other form gave that
  load. The matching guide forbids `volatile`, so the function stays parked
  and the exact form is kept outside the tree as evidence.

Tooling, all private:

- A parked candidate that needs fields or symbols the tree does not have
  now carries them in files next to it. The search, the retry setup and the
  recovery step lay them over their own copies of the tables; the tree
  gets a field only through a function that is exact with it. Seven parked
  candidates that the search could never compile do compile now.
- The search's preparation step shared one directory between parallel
  chunks, which gave a rare false "does not compile". It is per function
  now. One of the seven was that.

Lessons:

- **The package shape matters more than the model's effort.** The same
  functions had been retried before in groups of several per agent. One
  function, its search score, its size and one line of what is known
  matched 40 percent of them.
- A search result rejected for a leftover variable is one hand edit away.
  Read the rejected results after every sweep.
- A candidate that does not compile is invisible to the search. Check the
  sweep log for those first.

## Thirty-first group: the same call once per path, exact twins, eighth sweep

Counts at this snapshot. Nothing is running.

- 1,607 functions exact. 1,554 come from C, 268,396 bytes, and 53 from
  assembly, 1,040 bytes. Read-only data owned by units is 2,332 bytes in 19
  ranges, 68 bytes and one range more: one of the new functions has a jump
  table. Initialised data is unchanged at 5,900 bytes. 336,732 payload bytes
  stay raw. About 81 percent of the inventoried resident code bytes: 266,000
  of 328,096, counted as before.
- 25 functions are new, 5,720 bytes, all game functions of the resident
  inventory, all out of the parked pool.
- Against the inventory: 1,220 of its 1,302 game functions are exact,
  190,612 of 240,200 bytes, about 79 percent. The other 82 are parked.
  Library unchanged: 324 of 347. By area the count is 1,221 game functions
  and 386 library functions.
- Parked: 91 functions, about 51 KB (82 game, 9 library). Unattempted by
  the counting rule: 14 library functions, about 11 KB, as before.

Where the 25 came from:

- 18 from agents with one function each: 50 packages over 49 functions,
  one function twice.
  - 10 were the next by search score. A machine restart cut their first
    run after five minutes; the second run started from the files the
    first had left. 4 exact, 3 of them already exact in the file the
    interrupted agent left.
  - 21 more, most of them the next by score: 8 exact.
  - 3 that have an exact twin, named in the prompt: 2 exact.
  - 6 far candidates chosen by the sign of a merged call in the listing:
    1 exact.
  - 10 that had had such a retry before, now with the two lead lists
    described below: 3 exact.
- 4 written at the top level from the two findings below: 3 with the
  merged-call form, 1 in the form of its twin. Two of the three are
  functions on which an agent had just reported no match.
- 1 that the eighth sweep had found byte-exact and its own filter had
  rejected for a local that was assigned and never read. Without the local
  it is still exact.
- 2 from the eighth permutation sweep: 32 parked candidates, five minutes
  each. 5 more left with a closer candidate, 24 unchanged. The restart cut
  the sweep after 6 functions; the other 26 ran afterwards.

Finding one, the same call written once per path. This is the residual
kind that had no known source form, recorded so far as parameters copied
out of the argument registers at entry.

- Signs in the listing: a branch that lands directly on a call instruction
  whose delay slot is empty; before that call, a reload of the argument
  register or a store through it that only one path runs; a reload of the
  argument register before the first call of the function, although it
  still holds the parameter.
- Source: an early-return arm that ends in its own copy of the last call,
  `if (c == 0) { g(o); return; } f(o, 7); g(o);`, where the candidates had
  `if (c != 0) f(o, 7); g(o);`. The compiler merges the two calls, but only
  the call instruction. The argument setup stays with each path.
- A function with exactly this shape was already exact in the tree. A
  parked function with the same twenty instructions, apart from one
  constant and one call target, had been tried with other forms by agents
  and by the search.
- 8 functions are exact with it: 3 by hand and 5 by agents, most of them
  at the first attempt.
- A companion form: when one arm passes a constant literally and the last
  call passes a local that starts with the same constant, the local needs
  the callee's 16-bit parameter type. With `int` the compiler sees that the
  argument register already holds the constant and drops the arm's own
  load.

Finding two, exact twins.

- A private script compares the instruction sequence of every parked game
  function, with immediates masked, with every exact game function. Two
  parked functions had a twin at 100 percent, two at 90 percent, and 19
  more between 60 and 81 percent.
- Written in the twin's form (statement order, local types, separate
  externs against struct members), 4 are exact, and a fifth took its shape
  from the twin and was decided by a callee prototype.
- A second list gives, per parked function, runs of ten or more
  instructions that an exact function has too. 30 parked functions had one.
- Both lists go into every retry prompt now.

Other source forms that decided functions, each the last change needed:

- A callee prototype that takes the object, so that the argument register
  is set up before the call, and a second argument passed as a literal 0.
- A 16-bit value assembled with the low byte assigned first and the shifted
  high byte ORed in after it, for the operand order of one `or`.
- A halfword read into an `int` local through a signed cast, then copied
  into the 16-bit local and clamped, for a load, a branch and a register
  copy in that order.
- An array indexed inside the loop instead of a separate pointer local.
- `x++` on a global where the candidate had `x = x + 1`.
- Parameters taken as `int` and masked in place.
- The callee branch written first: `if (c == 0) { return f(o) != 0; }
  return 0;`.

Marks in the sources:

- 76 functions say that their form was found by the search, 6 more that it
  was found by the search and cleaned by hand, 22 that they were finished
  by hand from a candidate the search had reshaped, and 4 that they were
  written by hand in the form of such a function. The first mark still
  requires a sweep log that reports the match.

Still parked: 29 of the 49 functions, as their packages reported it.

- Three functions of the high byte family need the loaded halfword in one
  register and its copy in another, the other way round from what every
  form gives. In the three exact siblings a later read into the copy local
  decides the registers; these three have no later read. About 11,000
  enumerated variants of local types, declaration order and cast form gave
  nothing exact.
- Four neighbouring functions keep one value in two registers, mask a
  parameter twice and leave a comparison of a masked bit with its own mask
  unfolded. A fifth member of the family is exact through the search.
  Inferred, not proven: one shared source idiom, an inline function or a
  macro, that has not been identified.
- 19 of the 29 differ in seven slots or fewer: two registers swapped, or
  one instruction a slot early.

Evidence for the open question about `volatile`, unchanged in substance:

- The callee of the 72-byte function was read. It does not use the register
  that the extra load fills, so the load has no reader in the original.
  It has no exact twin that shows another form. The function stays
  parked.

Tooling, all private:

- The twin list and the shared-run list, rebuilt from the function table,
  the listing and the build configuration.
- The retry prompt has a one-function addendum: the lead lists, the search
  score and its meaning, and one scratch folder per agent.

Lessons:

- **Look for an exact twin before retrying a parked function.** The pool
  was worked function by function, and the tree already held the answer
  for several of them. The comparison takes a minute for the whole pool.
- A residual kind without a known form is a reason to search the exact
  units for the same instructions, not to enumerate source variants. This
  note was already in the private runbook and was not applied to this kind.
- Background work does not survive a machine restart. Agents leave their
  work in files, so a second run can start from them: read the newest work
  folders first after a restart.
- Agents that share a scratch folder overwrite each other's scripts. In
  this round one such script then wrote another function's body into the
  wrong unit file. The agents restored the files, and every unit file was
  checked to define its own function before banking. Each agent now has its
  own scratch folder and builds only under its own tag.
- An exact form from an agent can still be spelled more plainly. One
  function was exact with a nested double assignment; a comma expression
  with `|=` gives the same bytes and reads as ordinary code.

## Thirty-second group: far and large functions, ninth sweep, locals assigned twice

Counts at this snapshot. One agent package of the last wave has not
reported and is not part of these counts.

- 1,616 functions exact. 1,563 come from C, 271,324 bytes, and 53 from
  assembly, 1,040 bytes. Read-only data and initialised data owned by units
  are unchanged at 2,332 and 5,900 bytes. 333,804 payload bytes stay raw.
  About 82 percent of the inventoried resident code bytes: 268,928 of
  328,096, counted as before.
- 9 functions are new, 2,928 bytes, all game functions of the resident
  inventory, all out of the parked pool.
- Against the inventory: 1,229 of its 1,302 game functions are exact,
  193,540 of 240,200 bytes, about 81 percent. The other 73 are parked.
  Library unchanged: 324 of 347. By area the count is 1,230 game functions
  and 386 library functions.
- Parked: 82 functions, about 48 KB (73 game, 9 library). Unattempted by
  the counting rule: 14 library functions, about 11 KB, as before.

Where the 9 came from:

- 6 from agents with one function each: 47 packages reported, over 41
  functions.
  - 10 functions of 104 to 328 bytes that were far from exact and never
    had a package of their own: 2 exact, one of them in the form of an
    exact function that shares a run of instructions. In most of the others the agent found a mistake
    of structure in the old candidate (a missing counter, a store on the
    wrong side of a test, a wrong mask constant).
  - 10 of 388 to 868 bytes, also first packages: 3 exact, one of them
    rewritten in the form of an exact neighbour.
  - 10 of 708 to 1,528 bytes: none exact. Most reports say that the blocks
    match and that registers, the frame or a spill slot differ.
  - 1 sibling of a function solved at the top level: not exact.
  - 17 chosen because their residual shows one of the three forms below,
    6 of them for the second time in this round: 1 exact, 9 closer, 6
    unchanged, 1 not reported.
- 1 written at the top level with the value copy form below. Its last
  difference was the operand order of one sum.
- 2 from the ninth permutation sweep: the 46 parked game candidates that
  agents had changed, five minutes each. 16 more left with a closer
  candidate, 28 unchanged.

Three source forms, all about a local that is assigned more than once:

- **Value copy.** The original loads a word, copies it to a second register
  and shifts the loaded register in place, then stores one half from each.
  Source: load into a local, copy it into a second local, assign the
  shifted value back to the first (`t = load; v = t; t = v >> 16;`). A
  fresh local for the shifted value loses the copy.
- **A conversion that stays.** A truncating shift, an `andi 0xffff` or an
  unfolded compare of a masked bit with its mask stays in the original
  where the compiler would normally drop it. Source: one narrow local that
  is assigned at two places, for example the operand of a first test and
  then the operand of a second. The search found this in a function on
  which about twenty hand forms had failed.
- **One scratch local for a chain of values.** Where the listing uses one
  register for an index, then an extent, then a result, one local reused
  for all three reproduces the registers. In one function this alone moved
  the object parameter out of the first argument register, as in the
  original.

The model behind them, from a register allocation dump of the compiler and
from the results. Inferred from this compiler's behaviour on the
reconstructions, not a statement about the original source:

- Locals that die exactly once inside one basic block are given registers
  first, lowest free register first. Locals that are assigned or die
  several times, or live in several blocks, come later, by rank.
- The compiler forgets what it knew about the upper bits of a local that
  is assigned more than once. That is why the conversion stays.
- The residual recorded earlier as a parameter moved out of its argument
  register at entry is, where no merged call explains it, a consequence:
  the original has one more temporary alive in the first block, a
  temporary takes the argument register, and the parameter is pushed out.
  In the function solved at the top level both entry copies appeared by
  themselves once the value copy was right.

What was tested and did not hold:

- **Inline helper functions.** The idea that the unexplained copies are
  the formal parameters of small inlined helpers. Three agents tested it on
  three families with about 130 hand-written and 1,700 enumerated
  variants, after about 760 enumerated at the top level.
  No function became exact. A helper loses the value copy in every shape
  tried, and the entry copies it seemed to produce were a side effect of
  one more temporary, as above.
- **Other compiler flags.** One large candidate was compiled with thirteen
  other flag sets (`-O1`, `-O3`, and single optimisations switched off).
  Every one was further from the original than `-O2`. Tested on one
  function only.
- **The high byte family.** One of the three became exact in the ninth
  sweep, in the form of its exact siblings: the copy local is read back
  from the stored byte later. One is at 4 differing slots. The third has no
  later read and is unchanged after about 13,000 enumerated variants.
- **The four neighbours with two masks.** The three forms reproduce most
  of what was missing (the kept `andi 0xffff`, the unfolded compare, the
  copy of the table entry). What is left is the order of the registers.

Marks in the sources:

- 78 functions say that their form was found by the search, 6 more that it
  was found by the search and cleaned by hand, 25 that they were finished
  by hand from a candidate the search had reshaped, 4 that they were
  written by hand in the form of such a function, and 1 that its form came
  from a scripted enumeration of variants and was finished by hand.

Tooling, all private:

- A flag comparison script for one candidate, and a variant tester that
  compiles a function body in a third of a second without a project build.
  The second made the enumerations above possible.
- The retry prompt has the three forms, the model and the scratch local
  note.

Lessons:

- **A hypothesis written down early and not tested costs a day.** The
  reading of the function with the kept shift (one local for both tests)
  was noted at the top level on the first day and then left. The search
  found the same form a day later. Test a specific hypothesis at once when
  the tester makes it a one-minute job.
- A theory that explains every residual at once, like the inline helpers,
  needs a cheap decisive test before agents are sent after it. Here the
  first agent results showed the mechanism behind the apparent success.
- Ask for the allocation dump. One agent used the compiler's own dump and
  explained in one report what two days of variants had not.
- The yield of plain retry waves is falling: 20 of 50, 18 of 50, 6 of 47.
  The functions that are left need the model applied one by one.
- Agents that had the automatic search's rewritten candidate as their
  starting file found it broken in one case (locals used before they are
  set). Keep the last hand-written candidate next to it and say so in the
  prompt.

## Thirty-third group: one shared header for prototypes

No function count changes in this group. The private tree was rewritten
and rebuilt; the image and every owner are the same as in the thirty-second
group: 1,616 functions exact, the same build configuration byte for byte.

What changed:

- Units used to carry their own prototypes of the functions they call:
  2,335 prototype declarations for 866 functions in 646 unit files.
- 781 functions now have one prototype each in a shared header that 643
  units include. The units lost 2,039 prototype lines. 296 declarations
  for 89 functions remain in units; 85 of those functions are declared
  only in units.
- The matching guide says where prototypes live now and what a unit does
  when it needs another one.

How the one prototype per function was chosen. Nothing was decided by
reading: every candidate was put into all units that declare the function,
the tree was rebuilt, and the build said which units stayed exact.

The scope of that test is narrower than the totals above. Its scanner does
not accept a parenthesis inside a parameter list, so it saw 2,327
declarations of 864 functions. It missed 8 declarations that have a
callback parameter. They belong to 5 functions: 2 were not scanned at all,
3 were scanned through declarations of another form. The classification
below is of the 864 scanned functions. The 8 declarations and the 2
functions were not tested and stay in their units.

- 620 functions had the same prototype in every unit and in the
  definition. 615 moved as they were. 4 use a type that only their unit
  defines, and the build threw 1 out of the header.
- 93 functions had prototypes that differ between units. For 36 the
  definition's signature keeps every unit exact. For 33 another typed
  prototype does. For 21 only the form without a parameter list does: the
  callers pass different argument lists. For 3 nothing does.
- 151 functions had one prototype in all callers and another signature in
  the definition. For 94 the definition's signature keeps every caller
  exact. For 53 only the callers' prototype does, and the definition needs
  its own. For 4 only the form without a parameter list does.
- Of those 244, 166 are in the header. 615 and 166 make the 781. 78 stay
  in the units: the 3 without
  a common prototype, 66 where the callers' prototype cannot stand in a
  header that the defining unit also includes, because the definition has
  another signature, and 9 that the build threw out of the header for
  another conflict.

What this says, as far as it goes:

- For 130 of the 244 the definition's signature keeps every reconstructed
  caller exact. These disagreements can be consolidated: units were matched
  one by one, each with the prototype its author wrote, and the build does
  not need the difference. That makes the definition's signature compatible
  with all reconstructed callers. It does not make it the original
  declaration, and it does not show that the other prototypes were wrong.
- For 94 functions callers and definition cannot share one typed prototype
  and stay exact (66, 25 and 3). Inferred, not proven: the original
  declared these without a parameter list, or defined them in the old
  style with narrow parameter types, so that callers pass plain integers.
  The compiler's own behaviour is the only evidence; no original
  declaration is known.
- One unit keeps all its own prototypes and does not include the header:
  with the shared prototypes it stops being exact.

Tooling, private:

- A script that tries every candidate prototype of a function in all units
  and rebuilds, and one that writes the header, rewrites the units and
  backs off a function or a unit until the build passes. The first took
  about four hours of builds for 244 functions; the second about one.

Lessons:

- The first version of the rewriting script had no text for a prototype
  that only the definition spells, and silently kept 104 functions out of
  the header. The total did not reconcile with the table of test results
  (26 in the header where 130 should have been), and that is how it was
  found. Reconcile the output of a mechanical rewrite against the table
  that drives it before promoting.
- A change of this size needs no agent. A script and the build are enough
  when the build can judge every step.
- A count taken with the pattern of the tool it describes inherits the
  tool's blind spot. The first totals here came from the scanner's own
  pattern and were short by 8 declarations and 2 functions. The reviewer's
  independent count found it. Count with a second, simpler method before
  publishing totals of a rewrite.

## Thirty-fourth group: one shared header for data externs

No function count changes in this group. The image, every owner and the
build configuration are the same as in the thirty-third group: 1,616
functions exact.

What changed:

- Units declared the data they use themselves: 1,727 extern declarators
  for 732 data symbols in 646 unit files.
- 594 symbols now have one declaration each in a shared header that 644
  units include. The units lost 1,218 declarators. 509 declarators for 147
  symbols remain in units.
- 138 symbols are declared only in units. 9 are in the header and also in
  one unit that includes only the object header and was left as it is.
  594 and 138 make the 732.
- The matching guide has the rule for data externs next to the one for
  prototypes.

How a symbol qualified. This step tested no candidates. A symbol moved only
when every unit that declares it uses the same type and the same shape
(scalar, pointer, array) and the type is one the generated types header
defines. Then the tree was rebuilt.

- 49 symbols stay because units disagree on type or shape. An array in one
  unit and a scalar in another compile differently, so these are findings
  to resolve one by one, not noise.
- 87 stay because their type is a function pointer typedef that units
  define for themselves: `ObjectFn` for 72 of them and five other names for
  the rest, 160 declarators in all. These are callback tables, not struct
  data.
- 1 was thrown out of the header by the build, and 1 has a declaration the
  script does not parse.

Scope, counted with a second method that walks each extern statement
character by character:

- The counter calls "data" every extern declarator that is neither
  written with explicit function pointer syntax nor a function prototype.
  So the 1,727 and the 509 include the 160 declarators of callback tables
  whose type is a function pointer typedef.
- The separate count of 102 declarators for 72 symbols covers explicit
  function pointer syntax only, not all function pointer objects. The
  script does not handle those 102. They were not touched, and neither
  were 6 function prototypes written with `extern`.
- The totals above are from the second method, before and after, and agree
  with the script's own count of removed declarators.

Tooling, private:

- The script that writes the header and rewrites the units, with the same
  back-off as the one for prototypes, and the independent counter.

Lessons:

- The count of the previous group was corrected in review because it used
  the tool's own pattern. This group's totals were taken with a separate
  counter first. It showed one thing the tool's numbers did not: 9 symbols
  that are both in the header and in a unit.
- The script reported 87 symbols with a unit-local type. The first text
  of this group called those types structs without listing them. All 87
  are function pointer typedefs; the review found it. Name a category only
  after listing its members.

## Thirty-fifth group: callback typedefs and callback tables in the shared header

No function count changes in this group. The image, every owner and the
build configuration are the same as in the thirty-fourth group: 1,616
functions exact.

What changed, counted with the independent counter of the thirty-fourth
group before and after, and by listing the header's new lines:

- Five function pointer typedefs have one definition each at the top of the
  shared header for externs: `ObjectFn`, `HandlerFn`, `FrameFn`,
  `ObjectFnInt`, `UnitFn`. 68 typedef lines left the units: 60, 4, 2, 1
  and 1 in that order.
- 154 callback symbols moved into the header. 83 are declared with one of
  those typedefs: 72 with `ObjectFn`, 4 with `HandlerFn`, 3 with `FrameFn`,
  2 with `UnitFn`, 2 with `ObjectFnInt`. 71 are written with explicit
  function pointer syntax.
- The units lost 257 declarators: 156 that the counter calls data (the
  typedef-based ones) and 101 written with explicit function pointer
  syntax. Data declarators in units went from 509 to 353, explicit ones
  from 102 to 1.
- The header has 748 extern declarations now, 594 from the group before
  and 154 new.

What stays in units:

- `ScriptFn` has two definitions in the units, one returning a byte and
  one returning nothing, in 3 typedef lines. It stays, and so do the 4
  symbols declared with it.
- 64 data symbols are still declared in units: the 49 on which units
  disagree, those 4, 1 that the build threw out of the header in the group
  before, 1 that the first script does not parse, and the 9 that are in the
  header and also in the one unit left as it is.
- 1 symbol written with explicit function pointer syntax stays: it is a
  pointer to a function pointer, a form the script does not parse.
- One other typedef line stays in a unit. It is not a function pointer.

A property of the compiler that the step depends on:

- This compiler rejects a repeated typedef, even an identical one. A
  typedef in the shared header therefore had to leave every unit that
  includes the header, and the matching guide now says so.

Lessons:

- Listing the members before naming the category, as the review of the
  previous group asked, cost one script and changed two sentences here:
  the moved symbols are 83 typedef-based and 71 explicit ones, not "the 87
  callback tables", and 4 of the 87 stay because of `ScriptFn`.

## Thirty-sixth group: the data symbols that units declared differently

No function count changes in this group. The image, every owner and the
build configuration are the same as in the thirty-fifth group: 1,616
functions exact.

The test. 49 data symbols were declared with different types or shapes in
different units. For each, every declared form was put into all units that
declare the symbol, the tree was rebuilt, and the build said whether every
unit stayed exact: 108 builds.

- 19 symbols: every form that was tried keeps all units exact. For these
  the build does not choose.
- 18 symbols: some forms keep all units exact and others do not.
- 12 symbols: no form keeps all units exact.

What moved:

- The 37 symbols of the first two kinds have one declaration each in the
  shared header now. Where several forms pass, the one that most units
  already used was taken. That choice is a convention of this step. It is
  not a result of the build and says nothing about the original
  declaration.
- 70 unit declarations had another form than the one taken. The units
  lost 197 declarators. Counted with the independent counter: data
  declarators in units went from 353 to 156, data symbols declared in
  units from 64 to 27. The header has 785 extern declarations, 748 from
  the group before and 37 new.

The 12 without a common declaration, by the failure the build reported
for each candidate. Two classes were observed: a compiler failure, where
the compiler rejects a unit, and nonmatching code, where a unit compiles
to other bytes or to another size. A build stops at the first failure, so
a reported size mismatch does not show that every later unit compiled.

- 7: a compiler failure for every candidate. For 3 the candidates differ
  in shape, an array against a scalar. For 4 they differ in the struct
  type; one of these is a pointer declared with six struct types.
- 3: nonmatching code for every candidate. Two are scalars declared with
  three widths, one is an array declared with two element widths.
- 2: a compiler failure for some candidates and nonmatching code for the
  others.

What stays in units, counted the same way: 27 data symbols, which are the
12 above, 4 declared with the typedef that has two definitions, 1 that the
build threw out of the header two groups ago, 1 whose declaration the
first script does not parse, and the 9 that are in the header and also in
the one unit left as it is. And 1 symbol written with explicit function
pointer syntax.

A correction made in review: the private table marks every failed build
with one prefix. The first text of this group read that prefix as a compile
error and counted 8, 2 and 2. Two of those builds stopped at a size
mismatch, which is nonmatching code. Classify failures by what the build
reported, not by the table's label.

Reading, labelled as such:

- Inferred, not proven: the 12 are places where the reconstruction has not
  settled what the data is, or where the original source declared one
  symbol differently in different files. The build cannot tell these two
  apart. Each of the 12 is open.

## Thirty-seventh group: the reconstructed source is published

No function count changes in this group: 1,616 functions exact.

The decision. On 2026-10-05 the owner decided to publish the reconstructed
PS1 source in this repository, after asking what other decompilation
projects do and after the license of the reused library code had been
checked. Until then the rule was that no game-derived source enters Git
without an explicit review. This group is that review.

What is published, in `ps1/src/`: 760 files.

- 646 C files of game code and 4 shared headers.
- The Sony library part in `ps1/src/sdk/`: 50 C files, 5 internal headers
  and 42 assembly stubs.
- 8 assembly files for code that is assembly in the original: the program
  entry routine and small stubs.
- The build configuration, the symbol file, the field table, and the note
  on the library part's provenance with its license file.

What is not published, and why:

- 38 SDK headers that the library part needs to compile. They are
  unchanged copies from another project, sotn-decomp. 8 carry an AGPL tag
  there and 30 carry none in a repository whose license is AGPL-3.0. They
  are ignored by Git and stay local. The build needs them; a note in the
  library folder says where to copy them from.
- The executable, the compiler binary, disc extracts, analysis databases
  and the working folders, as before.
- The Windows research source. The decision covered the PS1 source.

Provenance of the library part, checked file by file against a checkout of
sotn-decomp at the commit the project has used throughout:

- All 180 files of that project's SDK folder carry an MIT tag. The 50 C
  files here keep it: 20 are unchanged, 12 are adapted to this game's SDK
  version, and 18 are parts of reference files cut out by this project's
  importer. Of the 5 internal headers, 2 are unchanged and 3 adapted. The
  42 stubs are generated from the reference's one-line stub files; they
  got the MIT tag line with this change.
- A note in the library folder names the project, the commit and the
  license, and lists the above. A license file in the same folder carries
  the MIT text with the reference's copyright line; the review of the
  publication asked for it, because the license's own condition is that
  the notice travels with the copies. The adaptations made here are under
  the same terms. This covers the library folder only.

The scan before the first push, over exactly the files to be added:

- No binary file. The largest file is the build configuration, 177 KB of
  text.
- No absolute path of this machine, no user name, no e-mail address, no
  credential pattern. One stray helper script of an agent with local paths
  was in the folder and was deleted.
- No file with an AGPL tag.

What changed in the tree for the publication. None of it changes the build
configuration's units or any compiled byte:

- The first comment line of 653 files said "Private reconstruction". It
  says "Reconstruction" now.
- The default configuration path of three tools points at `ps1/src/`. The
  tool test suite was rerun for that.
- The folder moved from the ignored workspace to `ps1/src/`. The relative
  paths in the configuration still resolve on this machine through two
  local links that Git ignores.

Verification: an uncached build from the new location gives 1,616 of 1,616
functions exact and the baseline's executable hash.

Open, for the owner:

- The repository has no license file. The published game source therefore
  carries no license of this project; the library part keeps its own. Which
  license, if any, is the owner's choice.
- A build from a fresh clone still needs the inputs listed in the PS1
  instructions. There is no import pipeline.
- Publishing reconstructed game source is common practice among
  decompilation projects and is a legal grey area. The owner was told so
  before deciding.

## Thirty-eighth group: one function at the top level, a 16-bit local scaled in place

Counts at this snapshot. They were recounted with a separate script that
reads the build configuration and the inventory; run on the previous tree
it gives the thirty-second group's figures.

- 1,617 functions exact. 1,564 come from C, 271,532 bytes, and 53 from
  assembly, 1,040 bytes. Read-only data and initialised data owned by units
  are unchanged at 2,332 and 5,900 bytes. 333,596 payload bytes stay raw.
  About 82 percent of the inventoried resident code bytes: 269,136 of
  328,096.
- 1 function is new, 208 bytes: `func_801386f0`, a game function of the
  resident inventory, out of the parked pool.
- Against the inventory: 1,230 of its 1,302 game functions are exact,
  193,748 of 240,200 bytes, about 81 percent. The other 72 are parked.
  Library unchanged: 324 of 347. By area the count is 1,231 game functions
  and 386 library functions.
- Parked: 81 functions, about 47 KB (72 game, 9 library). Unattempted by
  the counting rule: 14 library functions, as before.

What ran. The goals asked for the allocation model to be applied function
by function at the top level before more agent waves. This is the first
function done that way. Its parked candidate differed in 3 instructions,
and an agent's variant that was never banked differed in 1: the original
computes a scaled table byte into one register and copies it into a second
with `move`; that variant computed it twice.

- The compiler's dumps for the candidate and for variants: the first
  instruction list, the list after combining, the first scheduling pass,
  local and global register allocation.
- Variants by hand, each compiled and compared. They were not counted.
- A scripted sweep of 1,080 forms that keep separate locals for the table
  byte, the product and an unsigned 16-bit copy: none exact. The closest,
  8 of them, differ in 2 instructions.
- A scripted sweep of 114 forms in which one 16-bit local takes the table
  byte and is then scaled, half signed and half unsigned: 9 exact, all
  signed. The 9 are one form. Each scales with `t <<= 3` and compares `t`;
  they differ only in a cast or a multiplication by one on the compare, and
  6 of them carry an assignment to a second local that nothing reads.
- The retry of the parked pool with the real build accepted the function,
  and an uncached default build gives 1,617 of 1,617 exact with the
  baseline's executable hash.

The source that rebuilds the bytes has one local for the value:
`s16 t; t = table[index]; t <<= 3;` and every later use is `t`. The same
function with `t = t << 3` is exact too. With `t *= 8`, with `t = t * 8`,
with an unsigned 16-bit local, or with separate locals for the product and
its copy it is not. This is a compatible reconstruction. It does not show
how the original was written.

Two spellings of the rest rebuild the same bytes. The first exact one read
the object through another struct with a signed field and declared the
data itself as signed. The published unit takes the parameter type its one
caller passes, includes the shared prototype and data headers, and casts
to signed 16 bits at the five places that need the signed reading of a
field and of a global the headers declare unsigned. It was chosen because
it agrees with the shared declarations, not because it is known to be
closer to the original.

It did not carry over. The next closest parked function, `func_8014c9f4`,
also one instruction away and also a copy next to a shift, stayed where it
was through 96 forms: 84 over the types of two locals and seven spellings
of the shift, and 12 that reuse the copy local for an earlier read.

Inferred, not proven. Read from the dumps of this one function; not
checked against the compiler's source:

- The scheduling pass works backwards from the end of a block. Nearly
  every instruction that sets a register which is set only once gets the
  same top priority there. Among such equals the one that comes later in
  the source is placed later, and a load is taken before a non-load. So
  the order of two instructions can follow from the order of two source
  statements, and from whether a register is set once or more often.
- In global allocation a register avoids a hard register that a
  conflicting, lower-ranked register prefers. A register computed from a
  block-local temporary prefers that temporary's hard register. This
  decided which of two registers got `$v1` in several variants.
- Rank is refs, weighted by their logarithm, over the length of the live
  range. Both numbers are printed in the allocation dump. One instruction
  more or less in a range decided close cases here.
- A 16-bit mask or sign extension in a later block is dropped only when
  the compiler can follow the value back through registers that are each
  set once. A scratch local shared with a 16-bit field value kept the mask.

Corrections and lessons:

- The quick tester compares the disassembly of one linked function. It
  said "exact" for nine forms. Only the retry of the parked pool and the
  default build made it a count.
- The retry of the parked pool first stopped at once: a symbol fragment
  saved next to another parked candidate named 9 functions that units own
  by now, and the build rejects such assignments. The private helper that
  lays those fragments over a trial tree now skips lines for functions the
  trial tree's units define. A fragment saved from a batch folder is a
  snapshot; check it against the tree it is applied to.
- The retry tool rewrites the order of the symbol file. Only the one line
  of the new function was removed in the published tree; the set of
  remaining lines equals the trial tree's.

## Thirty-ninth group: the overlay map confirmed against the loader

The owner chose overlays as the next focus on 2026-10-05. The parked pool
stays as it is: 81 functions, 72 of them game functions.

No function count changes in this group: 1,617 functions exact.

What was open: the overlay map gave link addresses that were estimates from
the code inside the chunks, and said that the loader had not been read.

What ran, all static:

- A search of the executable's data for the estimated addresses found a
  table that lists them. A block of six table addresses refers to it.
- `func_801501a0`, already exact, indexes that block with two 16-bit fields
  of an archive entry. So the entry's first word is a slot and a table
  number, not one type, and the destination of a chunk is
  `tables[table][slot]`.
- `pac.py loadmap` (new) reads the tables from the executable and compares
  them with the estimates over all 239 archives: 1,572 chunks, 247 with
  code, in 16 slots of table 0. All 16 agree, none differs.
- `pac.py sides` (new) compares each second-side character block with its
  first-side twin: 391,211 words identical, 19,542 differing in a way the
  distance of 0x18000 explains, 57 other. 20 pairs; one pair differs in
  size by four bytes and was not compared.
- `pac.py loadmap --symbols` lists the symbols that the source assigns
  outside the image with the chunks loaded over them: 38 covered, 76 not.
- `test_disc_tools.py`: 26 new control cases on synthetic inputs, 62 in
  all, all as required.

Findings that change the plan for overlays:

- Four modules have one content across many files (24, 63, 42 and 21
  chunks). They are the cheapest place to start.
- Inferred, not proven: a second-side block is its first-side block
  linked at another address. The comparison supports it: 19,542 of the
  19,599 differing words fall into classes that the distance explains. 57
  do not and were not explained one by one, and one pair of different size
  was not compared. Four characters carry one more module with the same
  picture at another distance. The proposed approach for the build follows
  from that: one source and a second link. It becomes a fact for a block
  only when both links rebuild exactly.
- The resident tables of per-character entry addresses send character
  numbers 21 to 23 on the second side into the first-side block. That fits
  the three character files without a second-side twin.

Corrections to the earlier overlay page:

- It read the entry's first word as one 32-bit type. Exact code reads two
  16-bit fields.
- It said each character block starts with data. 17 of the 25 first-side
  blocks open a stack frame at their first word.
- It called two estimates weak. The table confirms them.

Lessons:

- Search the data before reading code for a table. The addresses the
  static pass had estimated were sitting in the executable as words; the
  function that uses them was then one search away and already exact.
- Agreement between two static sources is still static. The page says what
  an exact function does and what the bytes are; it does not say a load was
  observed.
- A comparison that explains nearly every difference is support for a
  reading, not the reading itself. The first text of this group and of the
  page said that the second side "is" a second link and that the tables
  exist as an "identical copy". Review corrected both: the first is an
  inference with 57 unexplained words and one pair not compared; in the
  second, the table contents are identical and the six address words are
  each 0x694 higher, which is exactly what makes them point at the copy.
- A one-off script is not evidence a reviewer can rerun. Each number on the
  page comes from a command of the published tool, or from an address and
  a count that can be read off the executable.

## Fortieth group: an inventory of the functions inside the overlay modules

No function count changes in this group: 1,617 functions exact. Everything
here is a static estimate; nothing was rebuilt.

What was open: the overlay map had no function boundaries.

What ran, all static:

- `funcscan.py` (new) finds function boundaries by a linear sweep. Its own
  text has the rules. `compare` checks a sweep against an inventory.
- Against the resident inventory of 1,649 functions: every start found,
  1,643 sizes equal. Five functions are longer in the sweep by the register
  restores and return that the inventory leaves out. The entry routine
  takes in the four table words after it. 202 functions are found only by
  the sweep.
- `pac.py functions` (new) sweeps each distinct content of a code-bearing
  slot: 77 contents in 16 slots, 11,220 functions, 1,461,872 bytes; 8,822
  distinct by bytes; 3,749 distinct with link addresses set aside, 677,624
  bytes. The [overlay map](../ps1/docs/overlays.md) has the table.
- Both commands print four counts that a wrong boundary can disturb. For
  the resident range and for the 77 contents alike: no `jr ra` outside a
  function, no function with two, no function with two frames, one
  function without a return at its end.
- `test_funcscan.py`: 156 control cases. `test_disc_tools.py`: 163, from 62.
- Mutation runs through `ps1/local/mutate.py`, each run under a time and a
  memory limit: 170 changes of `funcscan.py` and 102 of the new parts of
  `pac.py`, all noticed. Three of them only by a limit, because the changed
  sweep never ends; seven by a crash.

Findings that change the plan for overlays:

- The two sides share nearly all code: 2,127 functions in common between
  2,211 and 2,129. Characters share little with each other: 1,498 of the
  2,211 are in one first-side content only, and none is in all 24.
- The four modules with one content hold 959 functions, 129,348 bytes.
  The largest, slot `0x28`, has 683 functions that are 319 once link
  addresses are set aside.
- The resident inventory is short. The sweep reports 202 functions it does
  not list. One-off counts, not on the page because no command reproduces
  them: 19,748 bytes; 14 system call stubs and 22 lone returns; 136 are the
  target of a `jal` in the executable or the archives or have their address
  as a word in the executable; 63 are already named in `ps1/src`. Whether
  they enter the inventory needs the owner's decision.
- One call in the block of `PL17.PAC` targets a word inside another
  function of the same block. The sweep reports that function in two
  parts. The cause is not established.

What review of the pull request corrected:

- `compare` turned byte ranges and sizes into words before it checked
  them. A size that claimed up to three bytes beyond the file counted as
  zero padding, and a range that ended up to three bytes beyond the file
  was accepted. A range is now checked in bytes, must start and end on a
  word, and padding is checked byte by byte. The base must be a multiple
  of four and the offset not negative. `pac.py functions` refuses a
  destination that is not a multiple of four. The first counts of this
  group were 136 and 161 cases, 162 and 99 changes.
- The lesson about limits gave the host session's report as fact. It is
  now attributed, with what was and was not confirmed here.
- `PLAN.md` and the front page still gave 333,804 raw payload bytes, the
  value before the thirty-eighth group. It is 333,596.

What interrupted the work, as a session on the Windows host reported it:
the sessions ended four times on 2026-10-05 during a mutation run of
`funcscan.py`, because a changed copy looped while it allocated and the
machine ran out of memory, and each new session repeated the run from the
previous transcript. That session wrote the limited harness. The report
could not be confirmed from this machine: the kernel log of the earlier
boots is gone. Confirmed here: under the harness the same change grows
until it reaches the memory limit. The mutation list was written again
from the tool's rules, not recovered.

Lessons:

- A change that no case can notice often marks a condition that does
  nothing. Such conditions were removed instead of excused: four range
  tests that a set lookup or a `max` already made, a test that a table was
  empty, a branch clause for a coprocessor the machine does not have, and
  four branch opcodes that the instruction test rejects first.
- A description that names a standard is a claim about a table. The text
  said MIPS I while one set accepted four later branch codes. The accepted
  sets are now written out by mnemonic in a control case.
- Counts that do not depend on the method found the one real anomaly: all
  returns inside functions, one per function, one frame per function. They
  moved into the tool so that the page can cite a command.
- The first draft of the page again cited counts from throwaway scripts,
  the mistake of the thirty-ninth group. They were removed before the pull
  request. Check each number on a page against the command that prints it
  before writing the sentence.
- A result in a handoff note is a lead. The note gave 31 of 39 changes
  noticed for an earlier list; this group reran everything. The same holds
  for what the note says happened: write it as that session's report until
  it is confirmed here.
- Rounding down is a conversion that discards. Check a byte quantity
  before dividing it by four. No change of the mutation list could show
  the fault, because every case and every change used multiples of four:
  a mutation run measures the cases against the code, not the code against
  inputs nobody wrote a case for.

## Forty-first group: the owner's decisions of 2026-10-06

No function count changes in this group. Nothing was built for it.

The decisions, all by the owner on 2026-10-06:

- The design of the [overlay build proposal](../ps1/docs/overlay-build-proposal.md)
  is accepted as written, and the work goes ahead. Read as its decisions 1
  to 6: the baseline of a module, one configuration with several images,
  the second side as a second link, the naming rule, counting per image,
  and the two pilot targets in their order.
- The source of overlay modules is published like the resident source.
- The 202 resident functions that the sweep reports and the inventory does
  not list are sorted after the overlay pilot.
- All source that the project writes is published. The owner's stated
  basis: the project writes this source itself, and the owner holds the
  rights in it.

What the last decision changes:

- The rule until now was that the decision of 2026-10-05 covered the PS1
  source and everything else that is game-derived needed its own decision.
  Source the project writes no longer needs one. `AGENTS.md`, the
  requirements and the two target pages say so.
- The Windows reconstruction falls under it. It has not had the review
  that comes before a first push, the one the thirty-seventh group records
  for the PS1 source, and stays in its ignored folder until it has.

What it does not change:

- Not source the project writes, and still never committed: game binaries,
  disc extracts, analysis databases, decompiler output, compiler binaries.
- Third-party parts keep their own terms. The library part in
  `ps1/src/sdk/` is adapted from another project under its license and
  carries that notice. The SDK headers it needs stay unpublished.
- The repository still has no license file. Holding the rights in the
  source and granting others a license to it are two things; which
  license, if any, remains the owner's choice.
- The note of the thirty-seventh group on the legal position of published
  reconstructed source stands.

How the source is made, for the record: it is written by the project from
analysis of the game's machine code and compared with that code byte for
byte. No original source code is used.

## Forty-second group: module images in the matching build

No function count changes in this group: 1,617 functions exact. No module
unit exists yet; this is the first of the proposal's four tool steps.

What was open: the build knew one image, the resident executable.

What ran:

- The contract first: the section "Module images" of the
  [matching build](../ps1/docs/matching-build.md) was written before any
  code, with the control cases to require.
- `matchbuild.py` reads `[overlays]`, `[[image]]` and the unit key
  `image`, checks each image against the loader's table and its archive,
  applies the range rules per image, links each image alone and checks it
  against its chunk with the checks of the resident image. `--image` builds
  one. The report has one record per module image. `fndiff.py` refuses a
  unit of a module image until it knows them.
- The archive and table readers are those of `pac.py`, imported, not
  copied.
- `test_matchbuild.py`: 43 new cases, 157 in all. 38 are named for module
  images, one checks that a configuration without images reports none, and
  4 are cases on the comparison alone.
- The real configuration builds as before: 1,617 of 1,617 functions exact,
  the same report keys in the same order, and no key about images.

How the work was split. The top level wrote the contract and reviewed. A
subagent implemented it in two packages against the contract, each with the
control cases, a table of every new rule disabled in turn against the case
that must then fail, and a run of the whole suite. It was told to stop and
report on any gap in the contract instead of choosing.

What that review found:

- The contract said two things about one report key: present whenever
  `--image` is given, and absent without declared images. The implementer
  reported it instead of picking silently. The contract now says one thing.
- Disabling the whole-image comparison failed no case. No build can differ
  in a retained byte only, because retained bytes are copied from the
  baseline. The reporting of a comparison became one function, and four
  cases on it alone now fail when the whole-image line is removed.
- A chunk without bytes and a `table_pointers` that is not a multiple of
  four were not in the contract. Both are refused now, in the contract and
  the tool.

What review of the pull request found, both in error paths:

- An `image` key holding a number crashed the tool with a traceback and
  exit status 1, where a configuration error with status 2 belongs. The
  set of declared names was built from the raw value after its type had
  been rejected. It now comes from the validated list.
- A failed preprocessing, compilation or assembly of a module unit was
  reported without the image's name. The contract says every failure of a
  module image names it. The pipeline of one unit is now one function, and
  a failed step of a module unit gets the name at that boundary.

The owner's directions of 2026-10-06 that this change writes down:

- A fast loop for one unit is approved and comes next. Timed once on this
  machine with `date` around the command, not printed by any tool: a whole
  build with every object cached takes 35 to 40 seconds, and only a changed
  unit is compiled within it.
- After the pilot: sort the 202 resident functions outside the inventory,
  then a tool that labels game functions by the library family they call.
- For a port, if one is made, the game code stays as close to the original
  as possible and only Sony's library code is swapped. The requirements
  carry it. No port is planned.

Lessons:

- Write the contract before the code and hand out the code against it. Two
  of the three findings above came from the implementer having a text to
  disagree with.
- Ask for the table of disabled rules. A list of passing cases would not
  have shown the check that nothing could fail.
- A check that only a pure function can exercise needs a case on that
  function, not an artificial build.
- The table of disabled rules covered the rules that were written. The two
  faults the review found were in paths nobody had listed: a value of the
  wrong type under a known key, and a subprocess that fails. For each new
  key, try a wrong type; for each new message rule, walk every place that
  can fail.

## Forty-third group: a loop for one unit

No function count changes in this group: 1,617 functions exact.

What was open: every try on a unit ran the whole build. With every object
cached that build still preprocesses each unit for its cache key, links the
image, compares everything and runs every control. Only the changed unit is
compiled. The owner asked how efficient the loop is and approved a faster
one on 2026-10-06.

What ran:

- `fndiff.py --rebuild UNIT` runs the pipeline of that one unit into the
  build directory of an earlier whole build, prints its object checks,
  removes the tag's report and summary, and prints the diff. The
  [matching build](../ps1/docs/matching-build.md) has the contract, the
  [matching guide](../ps1/docs/matching-guide.md) the new loop.
- The pipeline of a unit and its preparation are now functions of
  `matchbuild.py` that a whole build and `fndiff.py` both call. No step is
  copied.
- A failed step keeps the previous object and the report: the unit is built
  in a scratch directory and moved into place only when its object exists.
- `test_matchbuild.py`: 15 new cases, 172 in all. The 168 of the first
  version ran in full and behaved as required; the 4 added in review are
  cases on one function.
- The real configuration builds as before: 1,617 of 1,617 functions exact
  and the same report keys.

Timed on this machine with `date` around the commands, not printed by any
tool: a whole build with every object cached 33.7 seconds, one
`fndiff.py --rebuild` of a real unit 0.72 seconds. The implementer measured
0.75 seconds when the object was not in the cache.

What review of the pull request found: the report was removed after the
unit's files had been replaced. An interruption or an error between two
replacements left the earlier "exact" next to a changed object, which is the
one thing the removal exists to prevent. The reviewer showed it with an
interrupt injected after the first replacement. The replacement is now one
function that removes the report first and replaces nothing if that fails,
with four cases on it.

What it does not do: decide. Only a whole build says a unit is exact, and
the removed report keeps an earlier result from being read as one. A changed
header is seen in the one unit only. Units of module images are still
refused; that is the next step.

Lessons:

- The first description of the cost was wrong in one word: "agents compile
  everything on every edit" was never true. The cache already limited
  compilation to the changed unit; the fixed cost was everything around it.
  Measure which part of a loop costs before changing it.
- A comment added to a source file does not change the cache key, because
  the preprocessor drops it. A timing meant to show a cache miss was a hit
  until the change was one the preprocessed text keeps.
- A safety rule has an order. "The report is removed" was in the contract
  and had a case; "before anything it describes changes" was in neither.
  When a rule exists to prevent a state, write down the moment from which
  that state must be impossible, and test an interruption there.
- This package ran 74 minutes in the subagent, with the cases, the table of
  disabled rules, a whole suite run and the timings in one brief. That is
  over what one package should take. Split the verification from the
  implementation next time, or cut the package in two.

## Forty-fourth group: module images in the diff and merge tools

No function count changes in this group: 1,617 functions exact.

What was open: `fndiff.py` refused a unit of a module image, and the pages
said that `mergeunits.py` did not know the `image` key.

What ran:

- `fndiff.py` compares a unit of a module image with the payload of its own
  image, beside the units of that image, with and without `--rebuild`. A
  failure of such a unit names the image.
- `mergeunits.py` needed no change. It writes back every key of a new unit
  table and treats every other table that differs from the base as a
  conflict. The sentence in the pages was wrong; five cases now show what
  it does for module images.
- `test_matchbuild.py`: 179 cases. The refusal case went and eight came for
  module units in the loop, three of them in review. The 176 of the first
  version ran in full. `test_mergeunits.py`: 39 cases, 5 new.

What review of the pull request found: three failures of a module unit still
came without the image's name. They were a link of the unit alone that
fails, a missing object and a failure while the files are replaced. It is
the fault of the forty-second group again, in another tool: a rule about
every failure, applied to the failures someone thought of.

Lessons:

- "The tool does not know this key" was written without reading the tool.
  It was a guess from the list of keys an older sentence named. Read the
  code before a page says what a tool cannot do, as before it says what a
  tool can.
- For a rule that says "every failure", list the places that can fail from
  the code, by searching for every raise, exit and failure print of the
  tool, and tick each one. Twice now a review found the ones that a list
  made from memory left out.
- The package for `fndiff.py` took the subagent three minutes: one tool,
  targeted cases only, the whole suite and the real build left to the top
  level. That is the size to aim for.

## Forty-fifth group: names across images

No function count changes in this group: 1,617 functions exact.

What was open: each image was linked alone and saw only its own units and
`symbols.ld`. Module code that calls the resident image could not link.

What ran:

- Every link is given the declared functions of the units of all other
  images as absolute addresses, in a file `others.ld` beside its linker
  script. The addresses come from the configuration, so one image builds
  alone with all names in reach. Only functions cross images.
- A name given that way must not be a symbol that an object of the same
  link defines. The check runs before the link, beside the `symbols.ld`
  check, and names the symbol, the unit and the declaring image.
- `fndiff.py` gives the link of one unit alone the same names.
- Without a declared image nothing is written and the linker script is
  what it was.
- `test_matchbuild.py`: 13 new cases, 192 in all. The 187 of the first
  version ran in full; the 5 added in review ran with the cases of the loop.

What review of the pull request found: the rule was applied in the whole
build and not in the link of one unit that `fndiff.py` makes. There an
address given for another image's function silently replaced a variable of
the same name in the unit, and the diff said nothing. The implementer had
reported exactly that as a judgment call, "the build check covers that",
and the top level let it pass. `fndiff.py` now runs the two checks of the
whole build before its link and a third for names of other units of the
same image, which a whole build leaves to the linker.

How the work was split: contract first, then one package for a subagent
with targeted cases only. It took five minutes. The whole suite and the
real build ran at the top level.

Lessons:

- A judgment call in a subagent's report is a decision waiting for the top
  level, not a note. "The other tool covers that" was accepted without
  asking whether the contract's rule had an exception for this tool. It
  had none.
- A rule about "every link" needs the list of links. There are two kinds:
  the link of an image and the link of one unit. Each rule for one must be
  asked of the other.

## Forty-sixth group: the pilot module is declared, its first function exact

Resident image: no change, 1,617 functions exact. Module image `slot2a`: 1
function exact, 20 bytes. The two are counted apart.

What ran:

- The real configuration declares its first module image: the one content
  of slot `0x2a`, taken from one of its 63 carriers, with the address that
  the loader's table gives. The build checks the chunk's hash.
- One unit of that image, the smallest function, 20 bytes: it increments a
  byte of an object. It is exact in the whole build, and `fndiff.py
  --rebuild` reports it identical. The module's other 21,176 bytes are
  retained raw.
- The whole build: 1,617 of 1,617 resident functions and 1 of 1 module
  function exact, both images' controls tripped.
- The control suite no longer assumes that the real configuration has no
  image. The two cases that need one without images strip them from their
  copy.
- The first real module unit showed a fault that no synthetic case had: the
  cache line of the summary counted resident units only. A case that
  compares that line with the number of units compiled failed on the real
  configuration. The line counts every compiled unit now.
- The archive is found through a local link `ps1/extract`, ignored by Git
  like the links for the executable and the compiler.

Conventions set here, in the matching guide:

- A module function that is not understood yet is `func_<address>_<image>`.
  Modules overlap in memory and names are unique over all images, so the
  address alone cannot name it. `symbols.ld` already has `func_801e0020`
  for a call of the resident image to that address; two modules have a
  function there.
- Module sources live in `ps1/src/<image>/` and include the shared headers
  as `"../game.h"`.

Lessons:

- A source in a subfolder that included `"game.h"` compiled, against another
  file: the SDK headers have a `game.h` too, and the include path found it.
  The first error came two includes later, for a header that the SDK does
  not have. Adding the config directory to the include path would have
  turned the trap around, onto the library units. An explicit `../` has no
  second reading.
- The sweep's count for this module is 28 functions with the entries that
  `pac.py functions` takes from the symbol file, 24 without. The listing
  for matching work has to be made the way the published inventory was.
- Building one module image alone takes under a second when nothing is
  compiled. It is the whole build of the loop for module work.

## Forty-seventh group: first round on an overlay module

Resident image: no change, 1,617 functions exact. Module image `slot2a`: 23
functions exact, 3,640 bytes, up from 1. Its other 17,556 bytes are retained
raw. The two images are counted apart.

What ran:

- Nine agents, one batch each, on the module's other 27 functions, with the
  method of the resident rounds: a work directory per batch, the module
  brief, and `fndiff.py --rebuild` as the loop. Each batch was then built
  again at the top level and only what that build calls exact was kept.
- 22 of the 27 are exact. With the first function that is 23 of the 28
  that the sweep counts. Five are parked with their candidates, 2,412
  bytes: 696, 1,084, 248, 100 and 284 bytes.
- The merged tree builds 1,617 of 1,617 resident functions and 23 of 23
  module functions exact; 1,645 and 24 comparator controls trip; the
  executable hash is the baseline's. `test_matchbuild.py`: 192 cases, all
  as required.
- 9 source files, 28 symbols and 49 lines of the field table are new. A
  struct and a symbol that only parked candidates used were taken out
  again: a field enters the tree only through code that is exact with it.

What the round shows about the sweep's inventory: every one of the 23 exact
functions has the start and the size that the sweep gave it. None of the
five parked candidates suggests another boundary. That is 23 boundaries
established by a rebuild and five still estimates.

The five that differ, as their agents left them:

- 696 bytes: the original stores to one byte twice in a row, zero and then
  0xff. The compiler drops the first store, 8 bytes short. The candidate
  suggests that the 32 bytes at the module's start are a table of this
  function.
- 1,084 bytes: the original keeps a separate tail for each of two branches;
  the compiler merges them, 80 bytes short.
- 248 bytes: two loop constants are loaded in the other order.
- 284 bytes: same instructions, other registers.
- 100 bytes: not judged by its agent, because its link failed for a reason
  in the setup, see below.

Times: the agents took between one and nine minutes each. The four batches
of small functions that came out whole took one to three minutes.

Mistakes in the setup, both mine:

- The work directories had no symbol for module functions that another
  batch owns. The resident rounds have one for every unmatched function.
  Two agents added the symbols themselves and flagged it; one did not, could
  not link, and reported nothing as exact although two of its functions
  were. A helper now adds them, and the base for the next round has them.
- The brief said that the build gives every unit the functions of the
  whole module. It gives the declared ones, and a function that no unit
  declares yet is not declared.

Lessons:

- Before a round, link one stub that calls a function of another batch.
  The trial before this round built one leaf function, which calls nothing.
- Two batches extended one struct with the same fields and other types,
  signed and unsigned. One batch still matched with the other's types, so
  the merge took those. Try that before deciding which side to believe.
- The loop is what makes a round cheap now: a try costs under a second, and
  a batch of five small functions is done in a minute.

## Forty-eighth group: second attempts on the module's parked functions

Resident image: no change, 1,617 functions exact. Module image `slot2a`: 25
functions exact, 3,988 bytes, up from 23. Its other 17,208 bytes are retained
raw.

What ran:

- The five parked functions each got a work directory with the candidate of
  the first round, built from the merged tree.
- One was read at the top level first: a single instruction differed, a
  signed load where the candidate had an unsigned one. A cast made it exact.
  Its agent had not been able to judge it, because of the setup fault of the
  last group.
- The other four went to one agent each, with the residual and a ranked list
  of ideas for it. One became exact: the two loop constants come out in the
  original's order when the masked copy of a pointer is taken into a local at
  the top of the loop body. The fix was none of the four ideas of its brief.
- The whole build: 1,617 of 1,617 resident and 25 of 25 module functions
  exact, 1,645 and 26 comparator controls tripped, the baseline's executable
  hash.

The three that stay parked, all now with the original's size:

- 696 bytes, 4 instruction slots differ. Both stores to the one byte are
  kept when the first goes through a second symbol for the same memory. The
  compiler then places that store elsewhere than the original has it.
- 1,084 bytes, 3 slots. The two tails were never the same text: one branch
  subtracts one from two values and the other does not. Writing that out
  gave the size. One constant sits in another register.
- 284 bytes, 7 slots. The loop counter and one mask have each other's
  registers.

Lessons:

- Read a one-instruction residual before handing it out. It took one
  minute at the top level.
- A ranked list of ideas in a brief helps even when the fix is not on it:
  three of the four agents reported which ideas moved the residual and
  which did not, and that is what the next attempt starts from.
- "The compiler merges the two tails" was the first round's reading of the
  large function. The tails differed in the source. When a residual is
  blamed on an optimisation, look for a difference in the code first.

## Forty-ninth group: the first side of the second pilot target

Resident image: no change, 1,617 functions exact. Module image `slot2a`: no
change, 25 functions. Module image `slot16`, new: 11 functions exact, 984
bytes, all that the sweep counts in it. Its other 10,406 bytes are retained
raw.

What ran:

- The image `slot16` is declared: the one content of slot `0x16`, which two
  character files carry. The build finds it in both.
- Three agents, one batch each, on its 11 functions. All 11 are exact. The
  agents took 27, 70 and 78 seconds.
- The whole build: 1,617 resident, 25 and 11 module functions exact; 1,645,
  26 and 12 comparator controls tripped; the baseline's executable hash.
- New: 3 source files, 3 unit tables, 6 symbols. No field.

What the module refers to outside itself, found before the round and given
names in the symbol file:

- One function in the resident image that the inventory does not list,
  `func_80149b80`. It is one of the 202 that the sweep reported; a module
  calls it.
- Two addresses in the character block, `func_801b606c` and
  `func_801b63fc`. The second is the address that lies inside a function in
  the block of another character file, the oddity that the overlay map
  records. Here a module calls it, so in the blocks of the two characters
  with this module it is the start of something callable. That is a lead
  for the oddity, not an explanation.

A finding for the shared prototypes: one unit had to leave `protos.h` out.
The header gives a resident function a 16-bit parameter, and with it the
compiler masks the argument before the call, which the original does not.
The unit declares the parameter as `int`.

What this sets up: the second side of this module, slot `0x17`, has the
same 11 functions at another address. Its calls into the character block go
to the second-side block, which lies at another distance than the module
itself. The second link has to give those two names other addresses for
that link.

What review of the pull request found: one unit declared a function of
another unit without a parameter and called it without one, while the
definition takes the object. The code was exact because the caller happened
to leave the object in the argument register. A scan of the module sources
then found four more declarations that differed from their definitions, all
in the first module: two in the pointer type, two in the width of a
parameter. All five now agree with the definitions, with a cast at the call
where the caller has another pointer type, and every unit is still exact.

Lessons:

- Exact bytes do not check a declaration. Two units can disagree about a
  function and both match. After a merge, compare every declaration of a
  module function in another unit with its definition; a private helper
  does it now. Where a caller only matches with another signature, that is
  a finding to write down. None of these five was such a case.
- The first round's review of this at the top level saw one of the five and
  filed it under the known cases where callers need another signature,
  without trying the definition's. Try it first.
- The setup of this round followed the two lessons of the first: every
  function of another batch had a symbol, and a stub that calls one was
  linked before the agents started. No agent reported a setup problem.
- Listing what a module calls before the round, by kind of target, found
  the three unnamed targets in a minute. An agent that meets one stops, by
  the brief.

## Fiftieth group: second links

No count changes in this group. Resident image 1,617 functions, module
images `slot2a` 25 and `slot16` 11. This is the last tool step of the
overlay build proposal.

What ran:

- A module image may be declared as like another. It takes every unit of
  the first image with each range moved by the difference of the two
  addresses, links the same objects again and is compared with its own
  chunk. Nothing is compiled twice. A unit that differs there fails the
  build and names the second link.
- One placement map per second link, for the units it links and for those
  it leaves out. A unit left out is raw there, and its names still come
  from the map at the moved addresses, never from the first image.
- A module image may give names of `symbols.ld` its own addresses, for
  what lies at another distance on the other side.
- A second link is given the names of `symbols.ld` that lie inside the
  first image at their moved addresses.
- `fndiff.py --image` compares a unit as a second link has it.
- The contract was written first and grew twice during the work, see
  below. Five packages for one subagent, between five and seven minutes
  each, each checked at the top level before the next.
- `test_matchbuild.py`: 233 cases, 41 of them new. The 230 of the first
  version ran in full; the 3 added in review ran with the second link cases.

What a trial on real data showed before the pull request, in a private copy
of the configuration:

- The second-side module of the pilot, declared as like the first, had 7 of
  its 11 functions exact at once. The other four differed in ten words.
- Three of the names behind those words are tables of the module that
  `symbols.ld` names and no unit owns. They have to move with the module.
  That became a rule of the tool: a name inside the first image moves.
- Three are in the character block, which lies at another distance on the
  second side. They got addresses in the second link's own table.
- With both, 11 of 11 functions are exact and all controls trip. For these
  11 the inference of one source and two links has passed its test.

What review of the pull request found: the moved names need the addresses of
`symbols.ld`, which the build had never read before, only the names. The new
code read them with Python's rules for an integer, and those reject a
leading zero. A line that the build accepted and the linker reads as octal
now ended the tool with a traceback, also in a configuration without any
second link. The integers are read as the linker reads them now, and one
that it would not read is a configuration error.

A second finding of the review, in the controls: four cases with a changed
second chunk only passed after another case had prepared the seed builds.
Run alone, each failed with a missing key, before the tool was even
called. The whole suite hid it, because some earlier case always prepares.
Reading the seeded chunks now prepares them, for every case at once, and
each case of the module fixtures was run alone in a fresh process.

Lessons:

- A case must pass alone. A whole run only shows that the cases pass in
  that order. Run each new case by itself once before a pull request; the
  filter that selects one case exists.
- Fix a setup dependency where the shared thing is read, not in the four
  cases that were caught. The accessor prepares; no case has to remember.
- A file has one owner of its format. `symbols.ld` is the linker's. The
  first time a tool reads a value from it, read the linker's rules for
  that value, and test a form that the two languages read differently.
- New code that runs for every configuration is not covered by cases of
  the new feature alone. The parser change ran on every load.
- Try the real case before the tool is declared done. The synthetic
  fixtures had every data symbol owned by a unit, so none of them needed a
  name of `symbols.ld` to move. The real module needed three.
- Two of the contract's additions came from that trial and from an
  implementer's question: the moved names, and which image a failed
  pipeline step names when a unit is compared as a second link has it. A
  contract written before the code is a first version.
- Asking the implementer to state, for each rule, that both kinds of link
  follow it kept the fault of the forty-fifth group from coming back: the
  moved names, the names of units left out and the per-image addresses
  each come from one function that both links call.

## Fifty-first group: the second side of the second pilot target

Resident image 1,617 functions, unchanged. Module images: `slot2a` 25,
`slot16` 11, and `slot17` with the same 11 linked again.

What ran:

- `slot17` is declared in `ps1/src/build.toml` as like `slot16`, with the
  chunk of slot `0x17` from `PL11X.PAC` as its baseline and three addresses
  of its own for names in the character block. No source was added and no
  unit was changed.
- The whole build passes. The second link reports 11 of 11 functions exact,
  984 bytes linked again, 10,406 bytes retained raw, and its image hash is
  the hash of the chunk. Its 12 comparator controls trip.
- The resident image and the other two module images build as before.

What it shows and what it does not: for these 11 functions one source
gives both sides of the block byte for byte. That is compatibility with
the two chunks, not knowledge of how the original was built. The rest of
the block is retained raw on both sides, and no other block has been
tried.

The pilot ends here. Its three parked functions of `slot2a` go to the
permutation search with the parked resident functions.

## Fifty-second group: the functions outside the resident inventory

No function count of the build changes. What changes is what the counts
are measured against.

What ran:

- `pac.py unlisted`, a new command: it sweeps the resident executable,
  takes the functions whose start the inventory does not have, and says of
  each its area, whether the build has it, and how the executable and the
  modules refer to it. The method and the table are in
  `ps1/docs/overlays.md`.
- 203 functions are outside the inventory, not 202. The earlier count
  swept to the inventory's end, and the code goes on for one more function.
  The command showed it: swept to the inventory's end, one function of the
  build was no start of the sweep.
- Swept to the end of the code, every word is in a function or is zero
  padding: 1,852 functions, 1,436 in the game area and 416 in the library
  area. The build has 1,231 and 386. The two old denominators, 1,302 and
  347, are the inventory's and stay what older reports mean.
- The per-chunk sweep of `pac.py functions` became a generator that both
  commands use. Its totals on the real archives are unchanged.
- `test_disc_tools.py`: 288 cases, 125 of them for the new command, and
  `test_funcscan.py`: 159 cases, on the head after review. The first cases
  were a package for one subagent, against the rules written in the tool
  and the overlay map; the expected output is worked out in the test from
  the layout of a synthetic executable, not read from the tool.
- Private mutation tables, each change run under a time and a memory
  limit, on the head after review: 152 changes to the new code and 174 to
  the sweep, every one noticed. The table of the `functions` command had 7
  entries that no longer matched the moved loop. They were rewritten; they
  and one new entry are noticed, as the other 95 were in the whole run
  before review.

What review of the pull request found:

- A unit that says `image = "resident"` was taken for a module unit. The
  build contract has two spellings of the resident image, no key and that
  one. The command read the configuration of another tool by one of them
  and would have reported rebuilt functions as not in the build.
- `--end 0` was taken for no `--end`: the code read `args.end or` the
  default. The sweep then ran over another range than the one asked for
  and reported success.
- The count of cases in the first version of this entry, 281 and 118, was
  read before the last case was added. The head had 283 and 120.
- Not blocking: a `j` or `jal` takes the upper four bits of its target
  from the address of its delay slot, not from its own. The new code and
  the sweep both used the jump's own address. No address of this game is
  affected. Both now call one function for it, with cases on each side of
  a 256 MB border for the jump's own word and the word before it.

Lessons from the review:

- A tool that reads another tool's file reads that tool's contract for
  it, defaults and second spellings included. `symbols.ld` in the fiftieth
  group and `build.toml` here are the same mistake.
- `value or default` is wrong for an option where zero is a value. Ask
  whether the option was given.
- A count goes into a text after the last change to what it counts. This
  is the fourth time; the rule now is: the count is read off the run that
  is made after the last edit of the test file, and that run is the last
  thing before the commit.
- When a fix adds a shared function, each rule of it needs a case that
  tells it from its neighbours. The first case for the delay slot told
  the jump's own address from the next word and not the next word from
  the one after; a changed copy of the tool showed the gap.
- The reviewer's note on pull request 56 is in the overlay map: the 8
  unexplained words of the pair of slots `0x16` and `0x17` are calls into
  the character block, which lies at another distance.

Lessons:

- A count made over a range is only as good as the range. The 202 came
  from a default that took the range from the inventory being checked.
  Check the range against a second source: here the build's own function
  table reached one function further.
- When a count can be reconciled, print the reconciliation. The command
  prints what lies between the functions it found, and that line is what
  shows that nothing else is left in the code.
- A refactoring of a counted command needs the controls run before
  anything else. Moving the chunk loop into a generator changed one number,
  the size of a chunk whose length is no multiple of four, and a control
  case named it at once.

## Fifty-third group: library families of the resident game functions

No function count of the build changes.

What ran:

- `families.py`, a new tool, and `ps1/src/library-families.toml`, the
  table it reads. A library function gets a family by its declared name,
  by the BIOS call of its stub, or by the folder of its unit under `sdk/`.
  A game function gets the families it calls directly and those it
  reaches through other game functions. The method, the table and the
  limits are in `ps1/docs/library-families.md`.
- On the resident image: 91 of 1,436 game functions call the library
  directly, 1,117 reach no library function, and 830 of those have no call
  on the way that the tool cannot follow.
- `pac.py` has one reader for the functions of resident units,
  `resident_functions`, which both tools use. The rule for which unit is
  resident is in one place.
- The names of five BIOS calls in the table were checked against the
  public PSX-SPX documentation. The reference project does not name them.
- `test_families.py`: 60 cases, a package for one subagent against the
  rules written in the tool and the page. `test_disc_tools.py` still has
  288. Private mutation tables under the limits: 147 changes to the new
  tool and 152 to `pac.py unlisted`, every one noticed.

What it found beside the labels:

- The game calls the thread functions of the BIOS.
- 116 library functions have no family, and more game functions reach one
  of them than reach any family. Identifying them comes before the labels
  can be trusted for naming.

## Fifty-fourth group: library families of the module functions

No function count of the build changes.

What ran:

- `families.py --modules` labels the functions of the overlay modules,
  each distinct content of a chunk taken alone. A module function calls
  functions of its own content and of the executable; any other target is
  counted and not followed.
- On the 77 contents, 11,220 functions: 121 call the library directly,
  7,008 call a game function of the executable, 8,105 reach no library
  function. 1,941 have a call through a register and 14 a call elsewhere.
- `test_families.py`: 75 cases. It had 60 before the modules; the same
  subagent wrote both parts. Private mutation table under the limits: 227
  changes to the tool, every one noticed.
- The review of the last pull request asked for other words about the
  closed functions: closed means that the tool found no call edge it
  could not follow, not that the function is free of the machine. The
  page says so now.

What the control cases found before the pull request: a module function
that calls a function of its own content at an address where the
executable has a function too was given what the executable's function
reaches. The stated rule says that an address inside the chunk is the
module's. The subagent that wrote the cases built that layout, saw the
difference, stopped and reported it with a minimal input instead of
bending the expectation. The line above it had the same fault for library
functions. No module of the game lies over the executable, so the real
rows did not change.

What it shows:

- The modules reach the library almost only through the executable. No
  module function calls a sound, disc, pad or thread function itself.
- Among the direct calls that the tool decodes, calls it cannot resolve
  are rare, so taking each content alone loses little there. Calls
  through a register are the real limit of the labels. Reworded after the
  review of the pull request: the 14 count functions with an unresolved
  direct target, not all calls between modules, since the targets of
  calls through a register are unknown.

Lessons:

- A rule that the real data never exercises is only as good as its
  synthetic case. The real run was right and the code was wrong.
- The brief's rule to stop and report at a contradiction between spec and
  code paid for itself here: the finding came back in four minutes.
- Before a design gets a harder case built in, measure how often the case
  occurs. Resolving calls from one module into another would have needed
  a rule for which content is loaded. A private count first showed 14
  functions in 11,220, and the rule was left out.

## Fifty-fifth group: a round on the game functions outside the inventory

The owner chose this as the next item on 2026-10-06, out of three
candidates. The other two are not decided: identifying the library
functions without a family, and more overlay modules in the build.

Counts, from the build and from `pac.py unlisted` on the new tree:

- 133 game functions attempted, all that the sweep counts outside the
  inventory and the build did not have. 118 exact, 11,784 bytes. 15 parked
  with candidates. None unattempted.
- Resident image: 1,735 functions exact, 1,682 from C with 283,316 bytes
  and 53 from assembly. 321,812 payload bytes are retained raw.
- By area the build has 1,349 of 1,436 game functions and 386 of 416
  library functions. Of the 1,302 inventoried game functions 1,230 are
  exact, unchanged.
- Parked game functions: 87, the 72 of the inventory and these 15. The
  retry of the whole parked pool against the new tree recovered none.
- The executable hash and the three module images are unchanged.

What ran:

- Eight agents in parallel, one batch each, two to eleven minutes: seven
  batches of several small functions and one function of 1,776 bytes,
  which was exact on its first build.
- The private listing and function table did not have these functions,
  because the first inventory had not found them. A private helper made
  both for the 203 functions outside the inventory; the inventory's own
  files are untouched.

Findings:

- All 15 functions without a counted reference are exact as functions of
  their own, the nine of 8 bytes as empty functions. That is compatible
  with separate functions and does not show what reaches them.
- One agent deleted an existing line of `symbols.ld`, a second name for
  an address, against the brief and without saying so. Banking failed at
  the link, on other units that use the name. The line was restored and
  the batch banked. A check of every batch for lines missing from the
  shared files found no other case.
- Several new units declare callees or data themselves because the shared
  headers lack them. Two disagreed with the shared headers when the pull
  request was opened; see the review below.
- By the agents' reports, not checked at the top level: parked candidates
  differ mostly in register choice or in the order of two blocks. Two are
  of another kind: one original keeps a load whose value is never used,
  and one keeps an address in a saved register for a single read.

What review of the pull request found, three points of consistency in the
new source, none of them a byte:

- `func_8014f918` was declared without parameters in `protos.h`, from the
  days when only a caller knew it, and its exact definition takes one.
  The unit had left the shared header out to compile. The shared prototype
  and the callback type of the one caller now say `int`, and both units
  stay exact.
- Two units had left `externs.h` out and declared a byte as signed, because
  the code reads it with a signed load. A second, incompatible declaration
  is not needed for that: the units keep the shared unsigned declaration
  and read through a signed pointer, and stay exact.
- Four units still carried the residual comment of a function that had
  been parked out of them, above a function it did not describe. The
  private helper that parks a function left the comment behind; it drops
  such comments from the kept unit now.
- Beyond the three points: 22 new units had no shared headers at all,
  because the stub they start from has only `game.h`. All 86 include both
  now and the build is exact, so none of them contradicts a shared
  declaration. The stubs include both from here on.

Lessons:

- A unit that leaves a shared header out to compile has found a
  disagreement. Resolve it in the pull request that finds it: try the
  shared declaration changed, or the access cast, before publishing a
  second declaration.
- Read the diff of every new file once for comments that no longer
  describe the code under them. A helper moved the code and not the words.
- Check the shared files of every batch for deleted lines before banking,
  not after a link fails. One `comm` per file does it.
- Functions that an inventory missed are also missing from everything
  derived from it. Before a round, look up one function of the round in
  each file the agents are told to read.

## Fifty-sixth group: second attempts on the round's parked functions

Counts, from the build and from `pac.py unlisted` on the new tree:

- 15 functions attempted again, the ones that the round of the
  fifty-fifth group had parked. 11 exact, 2,156 bytes. 4 stay parked.
- Of the 133 game functions outside the inventory 129 are exact now.
- Resident image: 1,746 functions exact, 1,693 from C with 285,472 bytes
  and 53 from assembly. 319,656 payload bytes are retained raw.
- By area the build has 1,360 of 1,436 game functions. Parked game
  functions: 76, the 72 of the inventory and these 4.
- The executable hash and the three module images are unchanged.

What ran:

- One function per agent, and one agent for three sibling functions with
  the same residual: 13 agents, each done in under four minutes. Each got the
  first attempt's candidate, its residual in one sentence, and the list of
  source forms that decided earlier functions.
- The private notes that pair a parked function with a similar exact one
  were made again on a listing in one disassembler's spelling, so that
  functions from both listings compare.

What decided the eleven, by the agents' reports and the exact files:

- A local reused for a second value, or one local per block where the
  first attempt reused one: six functions, among them the three siblings.
- Early returns in the original's order in place of one nested condition.
- A `goto` to a shared `return 0` where plain returns were kept apart or
  merged otherwise: two functions.
- A field updated in three statements in place of a pointer local.
- A call written with one more argument, because the original has a value
  in the fourth argument register at the call.

One function of these reads a local that is not set on every path. Making
it a fourth parameter was tried at the top level and gives other code, so
the local stays, with a comment that says what it is.

The four that stay parked, by the agents' reports: one differs in where
one store is scheduled; one in two blocks that the build merges; one is 4
bytes short around a constant kept in a saved register; and one original
keeps loads whose value is never used and a branch on a register that is
always zero, which neither attempt could reproduce.

Lessons:

- A second attempt with the residual named and the list of decisive forms
  recovered 11 of 15 in minutes. Do it right after the first round,
  before the candidates go into the long pool.
- A sentence about the residual that questions the first reading helped:
  two functions were exact once the agent stopped treating the candidate
  as nearly right and rewrote the shape from the listing.
- When a form looks like a trick, try the honest form once and record the
  result. The unset local looked like one; the parameter form differs.

## Fifty-seventh group: 32 library functions take their names from the reference

The owner chose identifying the library functions without a family as the
next item on 2026-10-06. Two other candidates stay undecided: more overlay
modules, and second attempts on the older parked game functions.

No function count of the build changes: 1,746 functions exact, the same
bytes from C, the same hash. What changes is where 32 of them come from.

What ran:

- The private identification compiles the reference reconstruction of the
  SDK with this project's compiler and looks for each function in the
  image. For 33 of the 116 functions without a family it reports the
  reference's function as equal in every unmasked bit, 27 as part of a
  whole reference file and 6 alone. All 33 were in the build already,
  written by round agents under placeholder names, so the importer had
  passed them over: it took a file only if the build lacked one of its
  functions.
- The importer got a mode that counts a function as owned only when a
  unit under `sdk/` owns it. With it, 32 of the 33 moved: 22 whole
  reference files and 5 parts of files. The project's own versions of
  those functions left the tree. Their callers still use the placeholder
  names, which `symbols.ld` keeps as second names for the same addresses.
- One is left out: the importer skipped its file because another
  identified file overlaps it.
- `families.py` on the new tree: 84 library functions without a family,
  down from 116; sound has 146, disc 36, graphics 78.
- Carried over from the review of the last pull request: the comment on
  the local that is not set on every path now says that reading it is
  undefined behaviour in C, and the comment in `symbols.ld` on the callees
  of `slot16` describes the two entries that are left.

What it shows and what it does not: the reference's source for these 32
functions compiles to the bytes of this game with this compiler. That
identifies the function as the same code as the reference's. It does not
show that the reference's source text is the original's.

Not published by the importer on purpose: it had written two part files
again with a working note from the private adapted copy, about a function
that is not in those parts. The published files were kept as they were.

Lesson:

- A helper's rule for what to skip is a decision that ages. "Skip a file
  whose functions the build already has" was right when only the importer
  owned library functions and wrong once rounds matched library code
  under placeholder names. The twenty-eighth group had the same lesson
  from the other side: read what a tool passes over.

## Fifty-eighth group: 28 library functions named by reference

No function count of the build changes and no source of a unit changes.

What ran:

- `librefs.py`, a new tool. It reads the relocations of the units under
  `sdk/` from the objects of a finished build and reports, for each
  library function, the names by which those units refer to its start.
  The build is exact, so where the reference's source calls a name, the
  original calls the address that the link gave the name.
- On the real build: of 416 library functions, 327 are declared by a
  unit under `sdk/`, 28 others are referred to under one name each, none
  under two, and 61 have no name.
- `families.py` takes a second name of a function from the symbol file.
  The family table lists the 28 names, each with the family of the
  reference file that defines it. Library functions without a family: 56,
  down from 84.
- The 28 names were in `symbols.ld` before this group. The private
  identification had put them there from the relocations of functions it
  found equal. What is new is a published command that shows the same
  from the build, and the labels that use it.
- `test_librefs.py`: 120 cases on the head after review, 66 before it,
  with unit objects written by hand in the test. `test_families.py`: 92
  cases. Both were packages for one subagent against the written rules.

What the control cases found before the pull request: a value in the
symbol file that is no integer at all, such as a word, ended the label
tool with a traceback. The rule said it must print a message. The reader
of linker integers answers "none" only for a bad octal number and raises
for the rest, and the build's own parser never lets such a value reach
it; the new reader of second names did. The subagent reported it with a
minimal input and left its four cases out; the fix and the cases went in
at the top level. The same reading showed that a name was taken from the
tail of a word that starts with a digit; the pattern is anchored now.

What it shows and what it does not: a name by reference says that the
reference's source refers to the function so at an exact place. It is not
the original symbol by that alone, and a reference that names the wrong
function at its only place of use would not be noticed.

What review of the pull request found: the tool read which name a
relocation is against and not its addend. A call to a name plus four, or a
data word holding a name plus four, counted as a reference to the start of
the function. The reviewer showed it with an assembled object. None of the
28 real names was affected: their relocations all have a zero addend. The
tool now works the addend out, for a jump target, a whole word and the
two halves of an address taken as a pair, and counts a reference only
when it is zero; what it cannot establish it leaves out. An object from
the real assembler with each form, with and without an offset, gives only
the three references without one. The subagent that rewrote the cases
found one more gap in the fix: a relocation that carries its addend
counted even when it lay outside its section, because nothing read the
word. It is left out now, like the other kind.

Lessons:

- A relocation is a name and an addend. Reading one without the other
  reads another fact than the one claimed. The claim was "refers to the
  start of the function"; the code checked "refers to the name".
- The hand-written objects of the control cases had only what the first
  version read. A fixture written from the code under test cannot show
  what that code does not look at. One object from the real assembler,
  with the forms an assembler can emit, would have.
- Evidence that a private helper produced is worth a second look for a
  form that the published build can show. The names came from a private
  compile of the reference; the same fact sits in the relocations of the
  published build's own objects.
- A similarity score is no identification for a small function. The
  private table offered one reference function as the near match of six
  different functions of eight instructions. Six functions whose near
  match scored 1.00 have another name by reference than that match.

## Fifty-ninth group: a family by place for 21 library functions

No function count of the build changes and no source of a unit changes.

What ran:

- `families.py` got a fourth rule. A library function with no family by
  name, BIOS call or folder gets the family of a reference file's folder
  when the nearest function below it and the nearest above it that units
  declare from a source under `sdk/` come from that same file. Parts of a
  file count as the file.
- On the real inputs 21 functions get a family so: 12 graphics, 5 disc
  and 4 sound. Without a family: 35, down from 56. Of the 381 with one, 39
  have it by name, 5 by BIOS call, 316 by folder and 21 by place.
- `--library-out` says in a sixth column which rule gave each family.
- `test_families.py`: 100 cases on the head after review, 96 before it.
  The cases for the new rule were a package for the subagent that wrote
  the earlier ones.

A gap that the cases brought up: a reference file directly in `sdk/`, with
no folder, was not an anchor in the first version, so a function declared
from it between two functions of another file got that other file's
family. The subagent tested what the code did and flagged that the stated
rule said otherwise. Such a file is an anchor now: it ends the stretch
between the two, and having no folder it gives no family. The tree has no
such file.

What review of the pull request found: the place applied wherever the
family so far was `unidentified`, not wherever no earlier rule had
applied. The table may give a name or a BIOS call that family in so many
words, and then the weaker rule overruled the stronger one and the row
said `place`. The rule is asked now, not the value. Nothing in the real
table says `unidentified`, so the real output is the same.

Lesson from the review: a value that the input can also contain is no
sign that nothing happened. The code already kept the rule that applied
beside the family, and tested the family.

What it rests on: the code of one object file is one piece of the image,
so what lies between two functions of an object belongs to that object.
The rule assumes that what the reference has in one file is one object
file of this game. The build does not show that, and one case speaks for
caution: a function that the reference keeps in one file of the disc
library sits here between functions of another file of that library. Its
family is the same either way. The page calls the rule an inference.

A private check that the data allows, not printed by the tool: 13
functions between two functions of one reference file already had a
family by name. In all 13 it is the family of that file's folder.

Measured before building: a first private count used the 28 names by
reference as anchors too and found 22. Using only what units declare
gives 21 and needs no table from names to files, so the rule uses that.

## Sixtieth group: 17 more library functions from the reference, placed by what they call

Counts, from the build:

- 1,747 functions exact, one more than before: 1,694 from C with 285,488
  bytes and 53 from assembly. 319,640 payload bytes are retained raw. By
  area the build has 387 of 416 library functions; game functions are
  unchanged at 1,360 of 1,436.
- 17 more library functions come from the reference's source. The units
  under `sdk/` hold 302 C functions now. 16 of the 17 were in the build
  under placeholder names, written by round agents; one, 16 bytes, was
  not in the build.
- `families.py`: 32 library functions without a family, down from 35.
  `librefs.py`: 344 named by their own unit, 20 by reference, none two
  ways, 52 without a name.

What ran:

- The private identification got a third way to place a reference
  function. Small functions, a wrapper of one call for example, have too
  few fixed words to be found alone and match in several places. A place
  counts now when the code matches there in every unmasked bit and every
  symbol the function refers to is already known and lies where the code
  at that place points. One such place makes the function exact there.
- It placed 17. The importer took all 17 as parts of their reference
  files, and the build is exact with them: a wrong sibling would link
  another callee and differ.
- The importer no longer copies a working note about a function that is
  not exact yet into a part file.

What it shows: eight of the 17 already had a name by reference, from other
units that call them. Each builds exact under that very name. Two
independent kinds of evidence agree in all eight cases.

A side effect that the place rule got right: one sound function had a
family by place, between two functions of one reference file. One of
those two neighbours turned out to belong to another file. The function
has no family by place any more. The rule depends on what is known of its
neighbours, and says so.

Lesson:

- When a matcher refuses a case as ambiguous, ask what else is known at
  that place. The bytes of two wrappers are the same; what they call is
  not, and the callee's address was already established.

## Sixty-first group: a second overlay module, the one of slot 0xb

The owner chose more overlay modules as the next item on 2026-10-06, once
the automatic ways of naming library functions were used up. Not decided:
finishing the library by hand, and second attempts on the older parked
game functions.

Counts, from the build:

- Module image `slot0b`: 54 of the 61 functions that the sweep counts are
  exact, 5,356 of 7,960 bytes. 26,080 bytes of the chunk are retained raw.
  24 archives carry this content. 55 comparator controls trip.
- 7 functions are parked with candidates, 2,604 bytes, after two attempts
  each.
- The resident image and the other three module images are unchanged:
  1,747 functions, the same hash; `slot2a` 25, `slot16` 11, `slot17` 11.
- 25 unit files, 38 symbols and 12 structs with 82 lines of the field
  table are new.

What ran:

- A first round of seven agents, one batch each, two to eight minutes:
  52 of 61 exact. Banking kept only
  what each batch's own module build calls exact.
- Second attempts on the nine parked functions, one agent each, with the
  residual named: two more exact, one of them only because its unit
  lacked two declarations after it was split from a larger file.
- Every one of the 54 exact functions has the start and the size that the
  sweep gave it.

Fixed at the top level before the pull request:

- Six declarations of four module functions in other units disagreed
  with the definitions. Five now agree, with a cast at the call where the
  units use different views of the same object. One stays on purpose and says
  so in the file: a caller that must not mask a byte argument. With the
  definition's prototype the caller has one instruction more than the
  original; with an int parameter the definition has one more. Both were
  tried.
- One agent had named a variable of the resident image with the module's
  suffix. It has the plain name now.

What the round shows:

- `symbols.ld` had names from the resident side for three addresses of
  this module before the module was touched. Two of those functions are
  exact now, so those two addresses have a name from the resident side
  and a name from the module. That is the state that `slot2a` already has
  for one function; calls are not renamed here.
- Two batches described the module's objects with a struct of their own,
  each a partial view of the same memory. The build is exact with both.
  Folding them into one is open.
- One function needs a stack frame 8 bytes larger than its code uses. It
  is exact with the stand-in that the matching guide allows for that, an
  unused local array with a comment that says what it is.

What review of the pull request found, three points of consistency and
no byte: a dispatch table declared with functions that take no argument
while its entries point at functions that take the object; one variable of
the resident image declared as a pointer to the game state in three units
and as a reference to an object in nine; and one table declared with two
element types.

The private check that had found the mismatched function declarations
looked at functions only. It now also compares the type of every data
symbol across the module units, and it reads each dispatch table from the
module's bytes and compares the table's parameter list with the
definitions of the functions its entries point at. Run on the tree, it
found what the reviewer found and more of the same kind: three data
symbols of `slot2a` declared in more than one way, from the pilot, and 24
table entries whose function has another parameter list than the table,
4 of them in `slot2a`. All are aligned now and every function is still exact: a
function that takes nothing gets the table's parameter, unused, and a
function that used another view of the pointer takes the table's type
and keeps its view in a local.

The seven that stay parked, by their agents' reports: three differ in
registers or in the order of a few instructions with the size right; two
differ in size, one because the build merges two branches and one because
it hoists a value that the original recomputes; one small one loads two
fields in the other order; and the largest has the right size and frame
now and 77 differing slots.

Lesson:

- Run the check for mismatched declarations on the merged tree before
  anything else. It is quick, and every mismatch it finds is a question
  about what the code really is: here one of six was not a mistake.
- A check covers the kind of thing it was written for. The reviewer found
  three mismatches of two kinds that the check did not look for; teaching
  it those kinds found the rest of the class. When a review finds one
  instance,
  write the check for the class and run it on everything, the merged
  modules included.
- Batches that describe one object each invent a view of it. The cost
  comes at the seams: tables and calls between batches. Give the next
  round one named view of the module's objects before it starts.

## Sixty-second group: a third overlay module, the one of slot 0x12

The owner's choice of 2026-10-06, more overlay modules, continued with the
one content of slot `0x12`: 120,192 bytes at `0x80010000`, 187 functions by
the sweep, carried by 42 archives. It is the image `slot12` of the build,
taken from `CONT00.PAC`.

Counts, from the build:

- Module image `slot12`: 177 of the 187 functions are exact, 26,016 of
  29,928 bytes. One unit owns 24 bytes of read-only data. 94,152 bytes of
  the chunk are retained raw. 179 comparator controls trip.
- 10 functions are parked with candidates, 3,912 bytes.
- The resident image and the other four module images are unchanged:
  1,747 functions, the same hash; `slot2a` 25, `slot0b` 54, `slot16` 11,
  `slot17` 11.
- 104 unit files, 160 symbols and 15 structs with 104 lines of the field
  table are new.

What ran:

- A first round of fifteen agents, one batch each: 158 of 187 exact by
  each batch's own module build.
- The merge of the fifteen batches failed with 12 conflicts in the field
  table, all of them between views of the module's records: one view of
  the object with two sizes, one offset with two types, one struct name
  for records of two sizes, two sets of field names for one record. An
  agent resolved them by fixed rules (the base's line stays; the object's
  view has the object's size; a name used for two records keeps the one
  more units use; the type more units use stays and the other units cast)
  with every batch's functions still exact, and then aligned the
  declarations across the merged units.
- Second attempts on the 29 parked functions, one agent each: 19 more
  exact. Some got a third message when a new form was found.

Two source forms were found at the top level, each after an agent had
reported some twenty forms without effect, and each decided more than the
function it was found on. Both are in the matching guide now:

- The tag word of a primitive is a bit-field store. Masks written by hand
  gave the right instructions with the two constants in each other's
  registers. `game.h` declares `PrimTag` for it, the one shared type that
  is written by hand, because the field table has no bit-fields. Four
  units of this module use it.
- A pointer held in two variables keeps a store through one and a load
  through the other in source order, and decides which register the
  pointer gets. The explanation is an inference from what the compiler
  emits.

Neither form has been tried on what was parked before: the 76 game
functions of the resident image, the 7 of `slot0b`, and the 3 of
`slot2a`. Some of those candidates mask tag words by hand.

The stand-in for an unused stack frame:

- Seven functions of this module are exact only with it: six from the
  second attempts and one from the first round, where the array had
  another name. The tree has 18 now.
- The sixty-first group says the matching guide allows the stand-in. It
  did not at that time: the guide forbade dummy constructs without naming
  an exception, and only the twenty-fifth group of this file recorded
  that this one is accepted. The guide states the exception now, with the
  condition (the ordinary reasons for a larger frame looked for first)
  and the fixed comment. Two older units had the array without the
  comment; they have it now.
- Seven of 187 is a lot for one module. The agents looked for a local
  whose address is taken, a struct on the stack and a call with more than
  four arguments, and found none. What the original source had there is
  not known.

The ten that stay parked, by their agents' reports: two keep stores and
loads in an order that no form reproduced, where the function has only
its parameter to reach the object; three differ in the registers or the
place of a few values with the size right; one lacks a register copy
after an addition;
one needs a value in a saved register across a call that nothing in the
candidate keeps alive; one lacks a second load of a global and the early
zero of an argument; one loads a halfword unsigned and then sign-extends
it where every form gives one signed load; and the largest, 800 bytes,
has the right size and frame and about 50 differing slots.

What review of the pull request found, and what the wider check then
found:

- Three points, all about declarations and none about a byte: a parked
  function declared with three parameter types in three units; a dispatch
  table whose third entry was defined with no parameter; and a function
  of the resident image declared with two return types in the new units
  and called without any declaration in one.
- The private check had compared a declaration only with a definition in
  a module unit, and had read only tables declared with the pointer
  syntax. The table of the second point was declared through a typedef,
  and its entry was parked when the check last ran. The check now
  compares every declaration in a module unit with the definition
  wherever it is, resident units included; requires one declaration for a
  function that no unit defines; reports a declaration without a
  prototype; has a compiler front end report every call of an undeclared
  function; and reads tables declared through a typedef, whose entries
  may be defined, parked or resident. Eight planted defects, one per kind
  of line, are each reported.
- Run on the tree, it printed 31 lines where the review had three points:
  19 declarations that differ from a definition, 3 undefined functions
  declared in more than one way, 8 calls of undeclared functions and 1
  table entry, in all four module folders. All are fixed and every
  function is still exact. 269 declarations in resident units also differ
  from their definitions; that backlog is counted and not touched here.
- An agent fixed them by fixed rules and left five declarations without a
  prototype, each with the note that both sides had been tried. None of
  the five was needed. One callee is exact with an `int` return, with a
  cast at the resident callers that mask the result. The other two, and
  the one such declaration that `slot0b` had since the last group, are
  exact with an `int` parameter copied into a narrow local, which is what
  "both were tried" had not tried. The tree's module units now have no
  declaration without a prototype. The form is in the matching guide.
- The agent's notes also claimed a measurement it had not made (the size
  of the definition with the caller's type). The measurement, made at the
  top level, is what showed that the exception was not needed.

Two control cases of `test_matchbuild.py` had been failing on the main
branch, and this round found it:

- The suite starts from the real configuration. Two of its cases remove
  the module images from a copy and expect the build to behave as it did
  before images existed. The helper that removes them dropped `[[image]]`
  tables and kept a table inside one. The pilot of slot `0x17` put the
  first such table, `[image.symbols]`, into the real configuration, and
  from that commit on both cases failed: what was left was no valid
  configuration.
- The suite's last full run on record is from before that commit. None
  of the twelve pull requests after it changed the tool or its suite,
  and the suite was not run in full for them. A copy of the main
  branch's source fails the same two cases, so this change did not cause
  it.
- The helper drops the tables inside an image now, and a case of the
  suite gives it a text with two images, each with a table inside, and
  requires what is left to parse and to hold no image. The helper as it
  was leaves a text that does not parse on that input. 234 cases, all as
  required.

Lessons:

- "Both were tried" is a claim about a search, and the search is as wide
  as the forms someone thought of. Before accepting an exception to a
  rule, measure each side at the top level and try the form that would
  make the exception unnecessary. An exception with a note is cheaper to
  write than to justify.
- A check that passes says what it looked at. The check for declarations
  looked at module definitions and at one way of declaring a table, and
  it passed while 19 declarations in module units differed from a
  definition. Give a check planted defects of every kind it claims, and
  count what it reads.
- A suite that starts from the real configuration is a check of the
  configuration too. Run `test_matchbuild.py` in full when `build.toml`
  gains a kind of table or key it did not have, not only when the tool
  changes. A new image is such a change.
- Naming the module's view of its objects in the brief was not enough.
  Fifteen batches still gave it two sizes and gave one name to records of
  different sizes. Before the next module's round, put the view's struct
  line with its size into the base, and tell the batches to name a new
  record type after the table it describes, not after what it looks like.
- A candidate's saved struct lines go stale when the merge renames or
  retypes what they rely on. The second attempts got a note that listed
  every change; no agent stopped on it.
- When an agent reports many spellings without effect, the residual is
  usually not about spelling. Both forms came from asking what would have
  to be true of the source for the compiler to behave as the listing
  shows, with one listing and ten minutes, not from more attempts. Pass
  the form on at once, to agents that can be resumed and to those not
  yet started: here it decided four more functions.
- A count typed into a brief is a claim like any other. The brief for the
  last pass said 177 before the build had printed it; it did print 177.

## Sixty-third group: four source forms tried on what was parked

The owner's choice of 2026-10-06, asked in plain words while the third
module was in review: try the new source forms on the 76 parked game
functions of the resident image before the next module.

Counts, from the build and from `pac.py unlisted`:

- Resident image: 1,754 functions exact, 1,701 from C, 288,624 bytes.
  1,367 of the 1,436 game functions are in the build; 69 are parked.
- `slot2a` 27 of 28, `slot0b` 56 of 61, `slot12` 178 of 187.
- 12 unit files are new: 7 of the resident image, 5 of module images.

The forms, all in the matching guide now:

- The tag word of a primitive as a bit-field (the sixty-second group).
- Two names for one pointer (the sixty-second group).
- The order of statements inside a block of the listing is not evidence
  of their order in the source. Found on a function of `slot0b` that was
  one instruction off: it is exact with the decrement, whose value the
  original tests from a register, directly before the test, though the
  listing has it three statements earlier. A heuristic, not a rule.
- A narrow parameter that callers do not mask is an `int` copied into a
  narrow local (found while fixing declarations in the last review).

What ran:

- The 12 parked functions of the three modules, one agent each, some
  twice: 5 exact. Two of `slot2a` by the bit-field form on the first try,
  two of `slot0b` and one of `slot12` by statement order or its like.
- The 76 parked game functions in 28 groups of two to four, one agent per
  group, each told to name the form a residual could be a sign of before
  trying anything: 7 exact. By the agents' reports: one by a callee's
  true parameter list and a computation moved behind the call, the only
  one of the seven in which one of the four forms had a part; one by one
  scratch local for two values; one by indexing the table at each use in
  place of a pointer local; one by a modulo in place of a mask and a
  constant held in a local; one by a spelling of an address; one by two
  pointer locals per iteration; and the largest, 1,376 bytes with 50
  slots open, by writing an update once in each arm of a switch after
  its agent had read the compiler's register allocation dump.
- So the forms carried over to the modules' parked functions, which had
  had one or two attempts, and hardly to the resident ones, which had had
  three or more and a search. What the resident round shows instead is in
  the reports of the other 69: nearly all are about which register a
  value gets, and two agents independently wrote themselves a script to
  read the allocation dump.

Cleaning the new units for publication:

- The seven resident candidates came from the parked pool and carried a
  first line that said "Private", no shared headers, declarations that the
  headers have, and leftovers of the permutation search. An agent brought
  them to the tree's conventions with each unit identical after every
  edit. Three keep a form that only a search would write and say so in
  the fixed line above the function.
- One of them had a local that was written inside an expression and never
  read. That is a dummy write. The form in which the local is also read
  for the two heights that have its value is exact as well, and is what
  the tree has.
- The declaration check now takes `--also` with resident units, so that
  units a round adds are checked like module units while the older
  resident units stay a counted backlog. On the merged tree it printed 14
  lines for the twelve new units; all are fixed. One callee got an `int`
  parameter where its one caller passes a signed halfword unmasked.
- A reviewer's nonblocking point from the last pull request is done: the
  one path of `func_80012d90_slot12` that passes an unset local says so.

What review of the pull request found: the guide's first wording of the
third form said that the compiler cannot keep a value read from memory
across a later store through a pointer, and concluded where a statement
must have stood in the original. A three-line probe by the reviewer shows
the compiler keeping a local across such a store. The paragraph now
reports the two functions as observations and the trying of another order
as a heuristic.

Lessons:

- A mechanism inferred from two functions is not a property of the
  compiler. Before writing "the compiler cannot" or "always" into the
  guide, write the three-line fixture that would show the opposite and
  compile it. The guide's own section on decrements does that; the new
  paragraph did not.
- A form found on fresh functions pays on functions that have had few
  attempts. On functions that have had many, what is left is what many
  attempts could not find, and a note with four forms is one more
  attempt. The estimate given to the owner before the round (a hit rate
  like the modules') was several times too high: 7 of 76 against 5 of
  12, and the 12 were functions of another kind.
- The residual that is left has one name in most reports: register
  choice. The thirty-eighth group already has a working model from the
  allocation dump, and agents rebuild a reader for it in their scratch
  folders each round. Make it a tool.
- A cleanup pass by rule can turn a variable that is read into one that
  is not. After a mechanical cleanup, read every form that the pass says
  it had to keep.

## Sixty-fourth group: a fourth module, at two addresses: slots 0x0 and 0x8

The owner's choice of 2026-10-06, more overlay modules, continued with one
character's extra module. Slot `0x0` of `PL09.PAC` and slot `0x8` of
`PL09X.PAC` hold the same 112 functions at two addresses, 17,220 bytes
each. They are the images `slot00` and `slot08`; `slot08` is declared as
like `slot00`, as `slot17` is like `slot16`.

Counts, from the build:

- `slot00`: 109 of 112 functions exact, 12,240 bytes; one unit owns 16
  bytes of read-only data; 4,964 bytes are retained raw. `slot08`: the
  same 109, linked a second time, exact against its own chunk, with no
  name of `symbols.ld` that needed another address on that side.
- 3 functions are parked with candidates, 864 bytes.
- 48 unit files, 52 symbols and 2 structs with 22 lines of the field table
  are new. The resident image and the other five module images are
  unchanged.

What ran, and what the changes to the method bought:

- A first round of eight agents on nine batches: 103 of 112 exact. The
  prompt was the third form: the forms note read before the first
  attempt, the object view's struct line with its size already in the
  base, record types named after the table they describe.
- The merge had 3 conflicts where the module of slot `0x12` had 12, all
  three the signedness of one 32-bit field, settled by hand in minutes
  with the two batches still exact.
- The declaration check on the merged tree printed 12 lines; an agent
  fixed them in 9 files. Four came from one mistake that the prompt had
  warned of: a declaration copied from another unit instead of from the
  definition.
- Second attempts on the 9 parked, three agents with three functions
  each and the patterns named in a note: 6 exact.

Findings:

- The sweep's first function of this content is wrong by 16 bytes: the
  content begins with a table of four words that decode as instructions.
  An agent stopped and reported it, as the prompt asks. The private table
  was corrected, the unit that reads the table owns it as read-only data,
  and the overlay page says that the rows of slots `0x0` and `0x8` are 16
  bytes too high.
- Two stores to one field: the compiler drops the first when they stand
  together and keeps both when another store stands between them, and it
  then emits the two next to each other. A fixture in the matching guide
  shows it. It decided one function here on the first try, and the parked
  function of `slot2a`, which has the same pair, has not had it yet.
- A function of the resident image that was declared with no parameter
  is called with the object by two functions of this module. An agent
  gave the two units a declaration of their own and took the header out;
  the first version of this change kept the header and wrote the two
  calls through a cast, with a comment that a port should call without
  the argument. Review showed that both were wrong about the code: the
  function forwards to the module's entry at this address, which the
  build now owns and which uses the object. The chain has one parameter
  list now: the function takes the object and passes it on, its resident
  caller passes it, and the two calls are ordinary calls. Every unit is
  still exact, the resident caller included, which had been assumed and
  not measured to need no argument.
- `protos.h` itself has 29 declarations without a prototype. They are
  part of the resident backlog that the declaration check counts and does
  not touch.

What review of the pull request found, besides the chain above: the
guide's paragraph on two stores said that such a pair "did not stand
together in the source". The fixture is right and the conclusion is not
proven: a `volatile` struct keeps both adjacent stores. The paragraph
now calls the move a heuristic, as the paragraph on statement order
does since the review before.

Lessons:

- Before writing that a callee "takes no parameter", read what it does
  with the argument register, and where it goes from there. Here it
  jumps on to a function that reads it, and that function had become
  part of the tree in the same change.
- "The other caller would need another instruction" was an argument, not
  a measurement. One rebuild of that caller would have shown that it is
  exact with the argument. Measure the alternative before building an
  exception around it.
- The second time in two reviews that a fixture's result was written up
  as a statement about the original source. A fixture shows what this
  compiler does with one input. Write "compatible with" and "a heuristic
  that worked N times", and never "the source had".
- The three changes to the prompt were each one sentence of a lesson of
  the last module round, and each paid at once: fewer conflicts, no
  untried forms left for the second attempt to find, record names that
  did not collide. Turn a lesson into the next prompt before the next
  round, not after it.
- Naming the pattern of a parked function in the retry message works when
  the pattern has a known form: the three with two stores, a missing copy
  and swapped operands went 3 of 3. The three named as register choice or
  load order went 1 of 3, and the three with the object copied into the
  argument register went 2 of 3 by a form that was not the one named: the
  copy was an argument.

## A port is started: the owner's decisions of 2026-10-06

No function count changes here. No game code was compiled for it.

The owner asked whether some of the PC port could be started, and decided
in the conversation that followed:

- It is for three systems: macOS on Apple Silicon, Windows and Linux.
- It is worked on in a worktree of its own.
- The first piece is the replacement of Sony's library, not a host build of
  game code, which was the assistant's first proposal.
- The replacement is PsyZ, used as a dependency. The owner asked what its
  MPL terms mean and was told that its headers carry no license before
  deciding.
- Discuss before coding the port itself. What followed is a pinned
  submodule, a check of it and pages; no source of a port.

Not a ruling: how the game's stored addresses are handled. The assistant
first recommended a block of memory standing in for the PS1's, then, after
the owner asked how sotn-decomp does it and it was looked up, recommended
that project's way (tables as C, when they are C) and that nothing be
decided before the library work needs it. The owner answered "ok" and went
on to the library. The [port page](../port/README.md) has both ways.

Corrections to keep:

- "Draw lists link their entries by PS1 address, so the graphics part
  forces a memory block" was said and was wrong: the reference project
  widens that link behind a build switch.
- "Its music player is largely stubs" was said of PsyZ from a comparison of
  names and was not established: PsyZ also builds decompiled library files
  for sound, and the comparison counted library functions that only the
  library calls. Compare what game units call, by address, through the
  inventory; that tool does not exist yet.
- The machine of these sessions has no development files for windows,
  graphics or sound and no CMake. CMake and Ninja were installed with pip
  into an ignored environment in the port worktree. A window cannot be
  shown from it as it is.
- `lane.sh` takes every checkout that is not the main one for lane B: run
  from the port worktree it would apply lane B's guard and write into lane
  B's scratch folder. The port lane does not run it. Its changes are
  checked by an empty `git diff origin/main...HEAD -- ps1/`, with three
  dots: with two, what the other lanes merged since shows up as a
  difference.
- The first check script was reviewed with three defects of the same
  kind: it had only been run in the folder it was written in. It was not
  executable in Git, because this machine's checkout does not record file
  modes (`git update-index --chmod=+x` sets one); it wrote its log into a
  folder that a fresh checkout does not have; and it changed folder with a
  relative build folder still in hand. A script gets its controls, with a
  fresh tree and stand-ins, before it is published.

What ran is on the port page, with the command.

## Completion map requested on 2026-10-06

The owner requested a completion map. `docs/completion-map.md` records the
post-PR-70 snapshot, linked from the README. It separates exact C, assembly,
owned data, parked candidates and retained bytes, and maps every analyzed
overlay slot. Function counts and code-byte coverage have separate static
denominators; there is no invented whole-game completion percentage. Second
links count as verified placements, not new source. Windows remains reference
research, and runtime observation is not a new acceptance gate for exact code.

The map is a current snapshot to update after ownership changes, not a rewrite
of historical evidence. It was authored in a separate worktree so concurrent
reconstruction and the 60-second PR watch can continue without interference.

The same day the owner found the text map insufficient and asked for a
picture in the manner of the AnyPS5 progress treemap: one square per
function, green or grey, grouped in blocks. `ps1/tools/coveragemap.py` draws
it from the inventories of `families.py` and `pac.py functions` and from the
build configuration, as `docs/completion-map.svg` with the counts in
`docs/completion-map.json`, embedded in the completion map and the README.
Decisions: the generator is written here, not copied, because AnyPS5 is
GPL-2.0; squares stand in address order, so the parked functions show where
they lie; the resident panel counts functions and the module panel function
placements, second links in a darker green and named in the heading; a
square is exact when units own all its bytes, data included, or own it
without a gap from one end with a function among them, which is how the
sweep's two known boundary errors (the table before the first function of
slot `0x0`, the table after the entry routine) count as exact without a
special case for either, while the raw bytes stay out of the byte count; a
swept function owned only as data is set aside and reported; a declared
function that touches no swept function is an error. That guard, review
noted, catches a declared function that the inventory does not represent
at all and nothing more: a row stale within its range, shorter or longer
than the function, passes (a control records a 16-byte function over an
8-byte row, exact at 8 of 8 bytes), and the sweep's boundaries stay
estimates beside the declared ranges, which the JSON gives next to the
bytes exact within rows. No address enters the SVG or the JSON. The counts
reproduce the text map's numbers exactly.

Asked why the picture was committed rather than drawn by GitHub Actions, the
answer was that the denominators come from the game files, which Actions
cannot have; the owner then chose to publish the inventory, `ps1/inventory/`
(addresses and sizes, no bytes), the one analysis table that is published.
`.github/workflows/coverage-map.yml` runs the control cases and `render` on
every pull request and push, and on a push to `main` publishes the SVG, the
JSON and a page to GitHub Pages, enabled through the API the same day with
the workflow build type. The SVG and JSON are not committed; the README and
the completion map embed the published picture. `sweep` is the one manual
step left, needed when a boundary moves; `render` fails the check only when
a declared function has no swept row at all, so running `sweep` after a
change to the symbol file or the configuration remains the round's duty.

Asked how to know the overall completion, the answer was that there was no
one number by design; the owner chose to add one. The bar above the panels
gives the share of distinct functions: resident functions once each, module
functions once per address-blind form (the hash of `pac.py functions`), a
form exact when any placement of it is. At that time 2,057 of 5,601, 36.7
percent, against 17.3 percent of placements and 19.5 percent of code bytes.
It is the measure of source left to write; the other two stand beside it.

The owner then asked for better names than `0x28` on the blocks of the
single-content slots, best effort. `sweep` now also writes
`ps1/inventory/contents.tsv`, the archives that carry each content, and the
map names a block from them: the archive family (`END`, `CONT`, `CDEMO`,
`DEMO`, `SELECTA`), the stems where three archives or fewer carry the
content (`PL09`, `PL11+PL13`), the first family and a count where more than
two families share one (`BOSS+2`, for BOSS, GDEMO and RDM). The slot and
the archive count moved to the tooltip. What the modules do is not inferred
from the names; the archive name is a fact of the disc and nothing more.
`render` refuses an inventory whose two module tables disagree, and draws
the old slot labels when the contents table is absent. Labels are never cut
short and are dropped when they do not fit, so the smallest blocks (slot
`0x1`, `CDEMO`) have their name in the tooltip only.

## Sixty-fifth group: a fifth module, at two addresses: slots 0x2b and 0x2c; the pilot module complete

The owner's choice of 2026-10-06, more overlay modules, continued with
another character's extra module. Slot `0x2b` of `PL0E.PAC` and slot
`0x2c` of `PL0EX.PAC` hold the same 107 functions at two addresses,
32,568 bytes each. They are the images `slot2b` and `slot2c`; `slot2c`
is declared as like `slot2b`.

Counts, from the build:

- `slot2b`: 102 of 107 functions exact, 9,052 bytes; 23,516 bytes are
  retained raw. `slot2c`: the same 102, linked a second time, exact
  against its own chunk, with no name that needed another address.
- 5 functions are parked with candidates, 1,192 bytes.
- `slot2a`, the pilot module: all 28 functions exact, 6,052 bytes. The
  last one was parked through three attempts with two stores to one
  field of which the compiler kept one; with a store to another field
  between the two it is exact, on the fourth order tried. Its unit owns
  32 bytes of read-only data.
- 37 unit files, 48 symbols and 1 struct with 15 lines of the field table
  are new. The resident image and the other six module images are
  unchanged.

What ran:

- A first round of six agents on seven batches, with the prompt of the
  last module and that module's units as examples of the same kind of
  code: 99 of 107 exact.
- The merge had no conflict. The declaration check printed 4 lines on the
  merged tree and 3 more when the second attempts and the pilot's last
  function came in, all fixed by hand in minutes; no declaration of a
  function differed from a definition this time.
- Second attempts on the 8 parked, three agents: 3 exact.
- The table unified one variable that two units of `slot2a` saw
  differently: one as four separate pointers, the other as an array of
  four. Both use the array now and are exact.

Findings:

- The private sweep of slot `0x2b` missed the first five instructions of
  one function, which opens its frame late; the sweep of slot `0x2c` has
  the function whole, and the published counts, which come from the
  public tool, are right for both.
- Several functions of this pair and one of the last are parked with a
  value in another register than the original's, in a block that sets
  up a call. No form moved it.

What review of the pull request found: three of the new units called a
function without the object that its definition takes, two calls of one
callee and one of another. The shared header declares those functions
without a parameter list, so nothing complained, and the three callers
are exact either way because the object is still in the argument
register. The private check now counts the arguments of every call in a
module unit against the definition's parameters. Run on the tree it
printed 25 lines, in five module folders: 22 calls of one function that
takes a slab pointer, and the three of the review. All 25 pass the object
now, each unit with the definition's prototype next to the header's
line, and every image is still exact. A planted defect of this kind is
among the check's controls, eleven in all with the unchanged tree.

The completion map: the owner added `docs/completion-map.md` in pull
request 71, asked on 2026-10-06 that it be updated with each progress,
and withdrew that a few minutes later: the owner will generate the map
by a tool. Rounds do not edit it by hand.

Lessons:

- A header line without a parameter list is a hole in every check that
  relies on the compiler. Twice now a review has named three instances
  of a kind that the declaration check did not look for, and the widened
  check then found 31 and 25. The 29 such lines of `protos.h` are the
  next place to look, before a review does.
- The second module of a kind is cheap: with the first one's units as
  examples and its lessons in the prompt, the first round gave 99 of 107
  and a merge without conflicts.
- A request that arrives during a round is acted on at the next step, and
  so is its withdrawal: nothing had been written into the map when it was
  withdrawn. Record both, so that the next session does not act on the
  first alone.

## Sixty-sixth group: a sixth module, the one of slot 0xf

The owner's choice of 2026-10-06, more overlay modules, continued with the
one content of slot `0xf`, from `DEMO.PAC`: 103,888 bytes at `0x800df000`,
228 functions by the sweep. It is the image `slot0f`.

Counts, from the build:

- `slot0f`: 215 of 228 functions exact, 32,712 of 36,912 bytes. Units own
  40 bytes of read-only data. 71,136 bytes of the chunk are retained raw.
- 13 functions are parked with candidates, 4,200 bytes.
- 107 unit files, 237 symbols and 19 structs with 121 lines of the field
  table are new. The resident image and the other nine module images are
  unchanged in their bytes.
- One function of the resident image changed its source: `func_8014f4d4`
  takes an `int` and copies it into a 16-bit local. Its fifteen callers
  are exact without a change, and a function of this module that passes
  an unsigned halfword unmasked is exact with it.

What ran:

- Before the round, a private helper listed which functions of the module
  are the same code, apart from addresses, as a function that is already
  exact: 63 of the 228, 37 of them in the module of slot `0x12` and 24 in
  the resident image. The agents were told to copy those from their twins
  and to change only names.
- A first round of 25 batches, eight agents at a time: 208 of 228 exact.
  The largest function of the module, 2,656 bytes, was exact on the second
  attempt of its agent.
- The merge had 21 conflicts. Eleven were one mistake: a batch's
  search-and-replace had changed a line in eleven structs of the base. An
  agent resolved all 21 by the rules of the reconcile brief.
- The declaration check printed 41 lines on the merged tree and 4 more
  after the second attempts; an agent fixed the 41 in 28 files, the 4
  were fixed by hand. Four symbols had been named after a wrong address
  and were renamed.
- Second attempts on 15 of the 20 parked, in five groups with the pattern
  named: 7 exact, one of them by the change to `func_8014f4d4`. Two
  became exact when a byte that other units reach as a member of the
  game state was written as that member and not as a separate variable.

What is parked and why, by the agents' reports: two need a resident
callee to take a wider parameter, and no form of that callee tried so far
is exact; three have the residuals of three functions that are parked in
the module of slot `0x12`, and look like the same code; two neighbours
have a base address in one argument register where the original has
another, and their parameter lists, read off the callers, did not move
it; the others differ in a few instructions each, one of them in the size
of its frame.

Lessons:

- Twins first. 63 functions of this module had a twin to copy from, and
  the batches that were mostly twins were the fastest of the round. Run
  the twin listing before every module round.
- What looks like a parked function is parked too. The three here failed
  where their likes in the module of slot `0x12` had failed. When a
  function becomes exact, try its form on its likes at once; when it
  stays parked, do not spend a second attempt on its like.
- Agents break shared files with search-and-replace: twice now a batch
  changed lines of other structs while editing its own. The merge caught
  both. Tell the next round to edit the field table by hand, line by
  line, and never with a pattern.
- A hint in a retry message is a hypothesis. "The first two parameters
  are still alive" was wrong for the three neighbours and cost one agent
  its whole time box. Say in the message that the hint is a guess and
  what would show it wrong, so that the agent drops it early.

## The module sweep offered every name to every module (2026-10-06)

Found while a seventh module was being prepared: the names of that module,
added to a private copy of the tree, moved a function boundary in the
inventory of another module that is loaded at the same address.
`pac.py functions` offered every address of `symbols.ld` to every module
whose range holds it, and took it as a function start wherever the words
there begin like a function with a frame. A data name of one module can
lie on the word where a function of another opens its frame some words
after its start, and the sweep then cut that function.

One row of the published inventory was wrong for that reason since the
names of the module of slot `0x0` entered the tree: the function of the
module of slot `0x2b` at `0x80079144`, 196 bytes, stood there as 176 bytes
from `0x80079158`. The matching build was not affected: it takes its
ranges from the configuration, not from the inventory.

The rule now: a name that ends in `_<image>` for a module image of the
build configuration belongs to that image's content (the chunk of its slot
with its hash) and is offered to it alone; any other name is offered as
before. `pac.py functions` and `pac.py unlisted` read the images from
`--config`, and `coveragemap.py sweep` hands its configuration on.
Controls: fourteen cases in `test_disc_tools.py` for the rule and three in
`test_coveragemap.py` for the hand-over. The numbers of the "Counts"
section of the overlay page were printed again with the rule; two of its
rows had also gone stale through names added in earlier rounds.

Lesson: an address in a module is not a name's whole identity where
several modules share the address. Whoever adds names for a module that
shares its address with others runs the sweep before publishing and looks
at every row that moved, in whichever module it lies.
## Sixty-seventh group: the first stage module, slot 0x6 of STAGE00.PAC

By the owner's decision of 2026-10-06 two sessions work at the same time,
each on its own modules. The owner's words: "we could assign half of the
overlays for the other agent ... there is no need for you both work on
the same shared spots". The second session has the stage files: slot
`0x6`, which has 20 contents, one per `STAGE*.PAC`, all at `0x801e8000`.
Their images are named `slot06_NN` after the number in the file name.
This group is that session's first.

Counts, from the build:

- `slot06_00`, the content of `STAGE00.PAC`: 49,896 bytes at
  `0x801e8000`, 28 functions by the sweep. 19 are exact, 1,392 of 5,248
  bytes. 48,504 bytes of the chunk are retained raw.
- 9 functions are parked with candidates, 3,856 bytes.
- 10 unit files, 18 symbols and 6 structs with 24 lines of the field table
  are new. The resident image and the other ten module images are
  unchanged in their bytes.

What ran:

- A private helper listed which functions of the module are the same
  code, apart from addresses, as a function that is already exact: 5 of
  the 28, all in the resident image.
- A first round of four batches: 18 of 28 exact. The merge had 3
  conflicts, all of one kind: one batch declared a 32-bit field where
  another declared its upper half.
- Second attempts on the 10 parked, four agents with the patterns named:
  1 exact.

What the code is, as far as the exact part shows it:

- The module begins with three functions. One calls a drawing function
  per layer under a bit of a flag word, one calls an initialiser per
  layer, one an update function per layer. The first stands at
  `0x801e8000`, the address in all 20 entries of the resident table
  `table_801725a0`; what calls the other two is not read. The three
  layers are the three records of 0x90 bytes from `0x801aa544` on.
  Resident units declare the first as `Chan`, and the resident functions
  that the stage code calls with these records take a `Cam` or a
  `Sprite`. The stage units use a view of their own, `Slot06Layer`.
- The tables that the update functions fill hold records of 0x1c or 0x20
  bytes. The resident functions that initialise one call `SetDrawMode`
  on the record and `MargePrim` on the record and its part at 0xc, and
  store a length of 3 or 4 and a code of 0x7c or 0x64 there. Inferred
  from that: a record is a drawing-mode primitive followed by a sprite
  primitive, of 16 by 16 pixels or with a width and a height. The shared
  types `Slot06Tile` and `Slot06TileW` say so, with a nested
  `Slot06Sprt16` or `Slot06Sprt`. They hold only the members that exact
  units use so far: three colour bytes, and the width and the height. Four batches had given the same record
  four names; the one type replaced them before anything was published.
- The words at 0x10 and 0x14 of a layer and of an object are read whole
  and as halves. The field table holds one of the two. The halves are
  declared, as `Object` has them, and a whole word is written through a
  cast.

Source forms, each measured by a rebuild:

- One variable changed in steps keeps what one expression loses. The
  drawing functions hold `(x & 0xffff) | (t << 16)` in a register and
  then shift it right by 16. Written as one expression, and with fresh
  temporaries, the compiler drops the `or` and the function is 4 bytes
  short. Written as `pos &= 0xffff; pos |= t << 16; y = pos >> 16;` it
  keeps it and the size is right. A local union of a word and two halves
  did not give this code in two tries: the frame grew by 8 bytes and the
  `or` was folded away.
- A pointer that is passed to a call after two stores is declared as an
  array and read as element 0. Declared as a scalar, the last store goes
  into the delay slot of the call and the function is 4 bytes short.
  This made the one second attempt exact, and a function of the next
  stage module before it.

What is parked and why, by the agents' reports and the instruction
diffs:

- The three drawing functions, 636, 628 and 636 bytes: the size is right
  and the loops are exact; 10 to 12 instruction slots of the first block
  differ, in the first function by their order alone.
- The function that the resident image calls to draw an object's tiles,
  700 bytes: 8 bytes short, with one saved register less than the
  original.
- Three functions with one difference of two layer fields each: the
  size is right and the operands are in other registers.
- One update function is 8 bytes long, one handler 8 bytes short.

Not claimed: what the stage's flag word, layers and tables mean beyond
the above, and that the other 19 stage modules are the same kind of code
throughout. `pac.py functions` counts 532 functions in the 20 contents
that are distinct apart from addresses, 481 of them in one content only.

Lessons:

- This code does not come as easily as the last modules. 18 of 28 in the
  first round, where the last two modules gave 99 of 107 and 208 of 228.
  The parked ones are not the long ones: three are under 220 bytes.
- When an agent reports many spellings without effect and a size that is
  4 bytes short, ask which instruction is missing and what would keep
  the compiler from folding it. The answer here was not a type but a
  variable assigned more than once, and it took one listing and a few
  rebuilds at the top level.
- Give a kind of module its shared types before the second module of
  the kind starts. Four names for one record after four batches is the
  cost of one round without them.
- A second attempt that ends with every instruction right and a few in
  another order is not a job for more spellings. An automatic search
  over the order of statements is being set up for those.

## Sixty-eighth group: the 20 stage modules

The second session's work on slot `0x6`, continued from the last group:
all 20 stage files are images of the build now, `slot06_00` to
`slot06_13`.

Counts, from the build and the published inventory:

| Image | File | Functions | Exact | Bytes exact | Raw bytes |
| --- | --- | ---: | ---: | ---: | ---: |
| `slot06_00` | `STAGE00.PAC` | 28 | 23 | 2,216 | 47,680 |
| `slot06_01` | `STAGE01.PAC` | 45 | 40 | 4,152 | 77,028 |
| `slot06_02` | `STAGE02.PAC` | 32 | 25 | 2,536 | 71,084 |
| `slot06_03` | `STAGE03.PAC` | 45 | 40 | 4,348 | 63,880 |
| `slot06_04` | `STAGE04.PAC` | 30 | 25 | 2,660 | 75,004 |
| `slot06_05` | `STAGE05.PAC` | 36 | 32 | 3,416 | 54,732 |
| `slot06_06` | `STAGE06.PAC` | 30 | 24 | 2,820 | 75,476 |
| `slot06_07` | `STAGE07.PAC` | 57 | 50 | 4,256 | 62,500 |
| `slot06_08` | `STAGE08.PAC` | 43 | 35 | 3,496 | 70,940 |
| `slot06_09` | `STAGE09.PAC` | 33 | 28 | 3,136 | 64,100 |
| `slot06_0a` | `STAGE0A.PAC` | 44 | 37 | 3,216 | 55,328 |
| `slot06_0b` | `STAGE0B.PAC` | 20 | 15 | 1,956 | 59,116 |
| `slot06_0c` | `STAGE0C.PAC` | 57 | 50 | 5,520 | 61,936 |
| `slot06_0d` | `STAGE0D.PAC` | 40 | 33 | 3,760 | 62,444 |
| `slot06_0e` | `STAGE0E.PAC` | 71 | 62 | 5,812 | 72,524 |
| `slot06_0f` | `STAGE0F.PAC` | 66 | 58 | 5,616 | 69,748 |
| `slot06_10` | `STAGE10.PAC` | 70 | 64 | 5,804 | 64,788 |
| `slot06_11` | `STAGE11.PAC` | 50 | 46 | 4,824 | 67,380 |
| `slot06_12` | `STAGE12.PAC` | 56 | 51 | 4,604 | 67,188 |
| `slot06_13` | `STAGE13.PAC` | 21 | 18 | 1,600 | 42,064 |
| all 20 | | 874 | 756 | 75,748 | 1,284,940 |

- "Functions" is the sweep's count in `ps1/inventory/modules.tsv`; the
  other columns are the build's lines per image.
- 118 functions are not exact. One-off counts from the private tables,
  not from a published command: 32 of them are parked with candidates,
  7,956 bytes, and 86 have not been attempted, 76,060 bytes, every one
  of them 600 bytes or more.
- 175 unit files, 456 symbols net and 10 structs with 94 lines of the
  field table are new since the last group. The resident image and the
  ten other module images are unchanged in their bytes.
- The coverage map's lines after this group: `modules: 1677/11220
  function placements exact`, `overall: 2670/5600 distinct functions
  exact (47.7%)`.

What ran:

- A first pass over every function under 600 bytes, stage after stage,
  eight agents at a time, 57 batches. Each stage's batches had the exact
  units of the stages before them in their work folder, and a private
  list of likes: for every function the exact function that is the same
  code apart from addresses, or the same instructions with other
  registers and constants, or the most similar one. By the round's
  notes, 716 functions were exact after it.
- Second attempts in groups by pattern, each after a form for the
  pattern had been found on one function: 40 more.
- A declaration pass on the merged tree: the check printed 102 lines,
  an agent brought them to zero in about 90 files with every image
  still exact. Thirty of them were one mistake: a caller that declared a
  layer's initialiser with another parameter type than its definition.
- A private helper made the tree for publication from the merged one:
  it drops declarations that a unit does not use, data symbols and
  field lines that no exact unit uses, and checks that nothing outside
  the stage lines of the three shared files changed.

Source forms, each measured by rebuilds, most of them on one function
and then exact on its likes in other stages within minutes:

- A narrow local that takes every step in place. The functions that set
  a layer's position from another layer's were parked in every stage
  with the right instructions and other registers. They are exact with
  one `s16` local and one statement per instruction: `d = l2->field_22;
  d -= l2->field_0a; d -= d / 4; d += layer->field_0a; d +=
  layer->field_36; layer->field_22 = d;`. An `int`, a local per value,
  or one expression give other registers. It decided 16 second
  attempts, and the later stages used it from the first attempt.
- A byte loaded into a `u8` local at the place where the listing loads
  it and stored back later, where the compiler otherwise moves the load
  to the top of the function.
- The function that fills two tables of tile records in two double
  loops, one per stage. The operand order of its address sums follows
  only when the arithmetic goes through `u32` casts: `(Tx *)(i * 0xfc0 +
  (u32)(t + j))`. Pointer casts and variables for the offsets do not
  move it.
- The same function is 8 bytes shorter in about half of the stages:
  there the listing forms the colour constant inside the inner loop,
  where the first form moves it into a saved register. It is exact with
  the two pointer locals assigned inside the inner loop. The dump of
  the compiler's loop pass shows why for one function: with the
  assignments outside, the inner loop has 21 instructions and the
  constant is moved out; with them inside it has 23 and the constant
  stays. Inferred from that and not proven for the original source:
  whether this compiler moves a constant out of a loop depends on the
  size of the loop at that moment. As a heuristic it worked on eight
  functions: where a build has a constant outside a loop and the
  listing inside, make the loop larger.
- The colour written as a literal in three stores against a local that
  holds it: both occur, by stage.

Three things to know about the published source:

- Three functions are declared `int` and return nothing
  (`func_801e828c_slot06_09`, `func_801e8170_slot06_0e`,
  `func_801e9410_slot06_0e`). Declared `void`, the compiler fills a
  delay slot with a write to the result register and the function is 4
  bytes short. Each carries a comment that says so. It is the one form
  found, it is compatible with the bytes, and what the original
  declared is not known.
- 12 functions of the stage units have the stand-in for an unused
  stack frame, with its comment.
- Three functions start before the row that the published inventory
  has for them, by 16, 16 and 8 bytes: `func_801e9cf4_slot06_07`,
  `func_801e9f10_slot06_08` and `func_801e9790_slot06_0f`. A name of
  the resident image points at the word where each opens its frame,
  after it has loaded one or two bytes of the game state, and the sweep
  cuts a function at such a name. The words before the cut end in no
  return, so the sweep set them aside. The build declares the functions
  with their first instructions and is exact; the inventory keeps the
  sweep's rows, as it does for the other known boundaries of this kind.

What the stage units need from outside their folders, all by added
lines: one field in `GameState` and one in `Config`; five names for
addresses outside the modules (`data_801904cc`, `data_80190542`,
`data_80190544` and two in the scratchpad). Two parked functions wait
for a resident function to change its declaration, which is the other
session's to decide: `func_801364a0` is called with the layer in the
argument register and declared without a parameter, and `func_80130768`
takes a 16-bit parameter that one stage caller passes on unextended.

What is not done:

- The functions of 600 bytes and more: three or four per stage that
  draw the layers, and one that the resident image calls to draw an
  object's tiles. The first stage's were attempted. The drawing
  function there has the right size, every instruction of the listing
  and exact loops; ten instructions of its first block stand in another
  order. A permutation search of 57 minutes, 24,000 orders of the
  block's statements and 6,000 ways of sharing scratch variables did
  not find the form. Statement order is not it: no order scored better
  than the first.
- 32 parked functions, 27 of them under 600 bytes, most with a few
  instructions in another order or register.

Lessons:

- A kind of code has its own forms, and they pay per kind, not per
  function. The first stage gave 18 of 28 in its first round. The last
  stage gave 18 of 18 in 80 seconds, with the same prompt and a note of
  forms that had grown in between. Write the note during the round and
  hand it to the next batch at once.
- One exact function is worth more than twenty close ones. Each of the
  three forms above came from one function that an agent happened to
  get exact in one stage while the same function was parked in ten
  others. Look at every report for a function that solved a parked
  pattern, and send that form to the parked ones the same hour.
- When every spelling fails, ask which pass of the compiler makes the
  decision, state a guess that a dump can refute, and give the agent
  the dump to read before the diff. The loop pass's count settled in
  twelve tries what twenty-five spellings had not.
- A scripted enumeration answers a narrow question quickly: 24,000
  variants in six minutes showed that statement order is not what the
  drawing function needs. It does not find a form that nobody put into
  the enumeration.
- Agents still retype lines of the field table with one replace over
  the file. Two batches did, after the prompt had said twice not to.
  The banking step's comparison with the base caught each. Keep that
  comparison; a sentence in a prompt does not replace it.
- A stray helper script that one batch leaves in its folder is merged
  as a new file and then stands in every later base. Look at what a
  merge copies besides unit files.

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
- A mutated copy can loop forever while it allocates. Run every mutant with a
  time limit and a memory limit, and end its whole process group on timeout:
  `ps1/local/mutate.py` does this. Confirmed here: under that harness two
  changed copies of `funcscan.py` grow until they reach its 4 GB limit.
  Reported by a session on the Windows host and not confirmed from this
  machine, because the kernel log of the earlier boots is gone: on
  2026-10-05 one such copy, run without limits, used all 31 GB of the WSL
  machine four times and ended every session.
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
- "These cases are independent" is a claim to test by running them side by
  side, not to read off their names. The control cases of `matchbuild.py`
  each have their own copy, tag and cache, but the cases of one synthetic
  fixture share throwaway seed builds under fixed names. The first parallel
  run failed 29 of 112 cases for that reason alone. Seed builds are now
  serialized per fixture, and a control case checks every fixture class.
