# PlayStation matching-C feasibility pilot

Date: 2026-10-03. Follow-up to the
[alternate-target audit](../../research/target-investigation.md) and
[Windows gameplay pilot](../../windows/docs/gameplay-pilot.md).

## Result

**All three selected functions rebuild byte-for-byte from readable C at their
original addresses: 592 bytes, 148 MIPS instructions.** No inline assembly,
embedded original bytes, comparison masks, or fallback substitution is used.
The reconstructed source is 74 lines including types and declarations.

This is the apparent PS1 counterpart of the same frame/box-table family tested
on Windows. Shared field offsets, record strides, control flow, and arithmetic
support that correspondence. This is more meaningful than selecting unrelated
easy functions, but not a controlled productivity benchmark: the PS1 work
benefited from the previous Windows reconstruction.

**Recommendation:** expand the PS1 matching source/build coverage.
It now has demonstrated matching-C evidence, not just attractive tooling.
Exact bytes at the correct addresses do not require a game boot or injection
test as an additional identity gate. Live observation is optional when it
resolves a semantic or loading question, not the next obligatory milestone.
The Windows family is behaviorally tested with modern Clang, but its historical
VC5 matching experiment has not been performed. This result does not establish
that Windows cannot match, that all PS1 code will be easy, or that PS1 is more
faithful to the arcade original. This work now belongs to the separate SFA2
repository; the old CPS2 workbench and its delivery contract were not imported.

## Baseline and family

User-owned Japanese Street Fighter Zero 2, main program `SLPS_004.15`:

```text
SHA256 0a522fd4791edfe95150edea057ca7113b8d0e93ed085be6854c042012c4a2a2
```

The PS-X EXE header specifies load address `0x80118900` after its 2,048-byte
header. All selected instructions are resident in this payload. No PAC
extraction or overlay mapping is required for this family.

| Address | Working name | Bytes | Result |
| --- | --- | ---: | --- |
| `0x80142128` | `build_metrics` | 524 | Exact C |
| `0x80142334` | `box_delta` | 36 | Exact C |
| `0x80142358` | `wide_delta` | 32 | Exact C |

The parent selects per-player table pointers, processes twelve slots, walks
terminated sequence records, calls both arithmetic helpers, and updates metric
and scratch fields. It is not an SDK wrapper or constant-return example.
Callers at `0x80129020` and `0x80131f7c` manipulate apparent player state and
initialization data. These are static role inferences, not original symbol
names or live-gameplay-confirmed meanings. The family has no external calls;
no callee is stubbed in the execution tests.

## Compatible toolchain, not unique historical identification

- GCC **2.6.3**, banner identifying the Sony PlayStation target, with `-O2 -G0`.
  This rebuilt Linux compiler runs under Supernova's existing Ubuntu WSL.
  No original proprietary SDK was installed.
- [maspsx](https://github.com/mkst/maspsx), commit
  `e85ecb373828aabea96d7e6400974d4f91ce8c74`, with
  `--aspsx-version=2.34`, converts compiler output for GNU assembly.
- GNU MIPS little-endian binutils **2.47**, assembling for R3000/O32 and linking
  at original addresses with explicit external-global addresses.
- Clang **22.1.1** for preprocessing and separate target-layout assertions only,
  not generation of the matching instructions.

The GCC 2.6.3 archive comes from SOTN's
[compiler-tools release](https://github.com/Xeeynamo/sotn-decomp/releases/tag/cc1-psx-26).
Its SHA-256 matches the checksum in the existing local reference checkout:

```text
ab156fe598ccad6aef704b8b35f3272de7edaf7eaab4e3d504c346ce6647df6c
```

Only build metadata/tool binaries from that reference were used, not SOTN game
source. GCC is an external GPL-derived tool; maspsx's MIT license was inspected.
Tool artifacts remain outside Git. Local manifests pin tools, scripts, source,
and the linker script. These checks establish copy integrity, not independent
authentication of an original Sony SDK.

Native macOS GCC 2.7.2 from
[old-gcc](https://github.com/decompals/old-gcc) was also tested. On the final source
it matches both helpers, but only 111 of the parent's 131 instruction words at
the same offsets. GCC 2.6.3 matches all 131. Both normal `-O2` and
`-O2 -fno-strength-reduce` matched the parent in the initial matrix; neither
`-O1` nor disabling instruction scheduling did.

This is a **working compatible toolchain**, not a uniquely identified original
compiler, SDK, flags, or assembler revision. Neighboring untested versions may
produce the same bytes, and other modules may differ.

## Source work and effort

The first typed candidate did not match. The useful changes remained readable:

1. Express the sequence walk as an inner loop with active-record and termination
   exits, matching the original block layout.
2. Preserve signed-short loop arithmetic and narrowing.
3. Express helper arithmetic as negated origin plus endpoint/extent, and load
   the wide-record base before advancing it. These affected compiler evaluation
   and register allocation without assembly constraints or dummy operations.

Existing Windows types transferred directly for the fields used here. Unknown
fields remain padding. The original chooses its frame pointer once before
walking the sequence; the C preserves that behavior.

Approximately twenty minutes covered this turn's setup, discovery, matching,
and verification before documentation. The first matching build occurred
earlier within that interval. This excludes preceding audits and Windows work.
Timestamps and failed candidates are retained. This is not a whole-game estimate.

## Validation

### Exact-byte gate

- Compiled and linked the three functions together from C, preserving original
  global addresses and internal call targets. Every byte, size, and start matches.
- Independently extracted the complete 592-byte ELF `.text` using GNU objcopy,
  extracted the reference range using `dd`, and compared with `cmp`: exit zero.
- Recompiled in a new build directory and repeated the independent comparison,
  without reusing old objects.
- Separately checked pointer widths, record sizes, and used field offsets with
  target-layout assertions.

The original and rebuilt complete family share SHA-256:

```text
51a800c583016f8856d32a3a82440142293a8e7dea0d425190ab9b70d9a9b73a
```

Compiler, assembler, and linker inputs do not include original game bytes.
The reference executable is read afterward by comparison and execution tests.

### Bounded execution checks

Original and rebuilt instructions were executed separately in Unicorn 2.1.4
on identical synthetic memory/register states:

- **5,120 cases passed:** 1,024 family cases and 2,048 direct cases per helper.
- All **148 instructions** and both outcomes of all **10 conditional branches**
  were covered, accounting for MIPS branch delay slots.
- Compared the full 128 KiB fixture, output-global page, scratchpad mapping,
  integer helper results, saved registers, stack balance, and live caller stack.
  Undeclared writes and execution outside the family fail.
- Covered player sides, noncanonical nonzero side values, signed boundaries,
  selector combinations, index truncation, terminated sequence lengths, and
  permitted read-only table aliases.
- **Eight C mutations** failed both byte and execution comparisons: two sign
  reversals, signed extent, wrong index mask, wrong maximum comparison, omitted
  slot, wrong-side frame table, and zeroed wide result. Each produced an actual
  output mismatch, not merely a build or emulator error.

Exact bytes are the primary evidence. Executing identical code is not an
independent proof of emulator accuracy. Unicorn's generic MIPS32 mode is not a
cycle-accurate PS1/R3000/GTE environment. Tests require initialized inputs,
non-overlapping inputs/outputs/live stack, terminated sequences, no interrupts,
and no concurrent writes. They do not cover all possible aliasing behavior.

**No PS1 game was booted, no rebuilt disc was run, and no live-gameplay
breakpoint or state capture was performed.** Other overlays, rendering, sound,
timing, SDK functions, and whole-game reconstruction remain outside this pilot.

## Private reproduction

Ignored root: `ps1/local/matching-pilot/`.

- `CONTRACT.md`, `metrics.c`, `pilot.ld`, `layout-check.c`: scope and source.
- `discovery/`, `original/`: function/body inventory, Ghidra output, callers,
  and first candidate.
- `scripts/build.py`, `compare.py`, `test_metrics.py`, `mutate.py`: actual builds,
  strict comparisons, execution tests, and negative controls.
- `matrix-results.json`, `final-comparison.json`, `clean-rebuild.json`,
  `execution-results.json`, `mutation-results.json`: evidence.
- `independent-check/`: separately extracted reference and rebuilt ranges.
- `artifact-manifest.json`, `experiment.json`: hashes, tool sources, and timing.

GCC 2.6.3 and per-build inputs/outputs are also private at
`C:\Users\dudu\Tools\sfa2-ps1-pilot` on Supernova. The build script uses an
authenticated SSH connection through the ignored `ssh.sock`. Credentials are
not stored. The new repository has a fresh root `.venv`, using the same pinned
Unicorn, Capstone, and pyelftools versions. It does not depend on the old
repository's environment. Historical reports retain their original paths;
see the [migration record](../../docs/migration.md).

From the repository root, using the existing private inputs and tool paths:

```sh
PY=.venv/bin/python
P=ps1/local/matching-pilot
ssh -M -S "$PWD/$P/ssh.sock" -o ControlPersist=300 -fN dudu@supernova.local
$PY "$P/scripts/build.py" "$P/metrics.c" reproduced --compiler 263
$PY "$P/scripts/test_metrics.py" --elf "$P/build/reproduced/metrics.elf"
$PY "$P/scripts/mutate.py"
ssh -S "$PWD/$P/ssh.sock" -O exit dudu@supernova.local
```

MIPS binutils was installed through Homebrew. No new VM, SDK, browser, emulator,
or game installation was created or modified. Game data, reconstructed source,
disassembly, and compiler binaries must not be staged. Only this descriptive
report and documentation links are intended for version control. No shared
workbench implementation or CPS2 baseline was changed by the pilot.
