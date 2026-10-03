# SFA2 alternate target investigation

Date: 2026-10-02. Scope: inspect the user's available Windows, PlayStation, and
Saturn copies for a more practical C reconstruction target. This is a static
binary audit, not a completed decompilation or a change to the CPS2 delivery plan.

Follow-up: the [bounded Windows gameplay C pilot](../windows/docs/gameplay-pilot.md)
completed on 2026-10-03. The observations below describe the preceding static
audit; the pilot records subsequent execution tests and their limitations.

Later follow-up: the [PS1 matching pilot](../ps1/docs/matching-pilot.md) reproduced the
corresponding three-function family exactly from C. It now supports expanding
PS1 matching-source coverage. Live gameplay observation is optional for
semantic/loading questions, not a required gate
for byte-identical code. The original static-audit recommendation
below is retained as history, not a claim that Windows proved easier than PS1.

## Original static-audit recommendation

**Use the Windows executable for the next small feasibility pilot.** The user's
GOG copy, original-CD installation, and mounted CD have byte-identical game
executables. The executable has
strong evidence of native, compiler-generated C/C++ game code, debug-runtime
remnants, and relatively uniform, apparently unoptimized function bodies. This
is a materially better starting point for exploring matching C than assuming
handwritten CPS2 assembly must be reproduced by a C compiler.

However, **no original debug-symbol file was found in either inspected
installation or on the mounted CD**. A developer PDB pathname survives, but the
PDB itself was not found.
The precise compiler, flags, source language per module, and achievable matching
rate remain unverified. There is no byte-matching C result from this audit.

The Windows port can also help explain CPS2 behavior. It cannot replace CPS2 as
the behavioral authority for an arcade reconstruction: port changes, regional
differences, timing, and altered data layouts require independent checking.

## Inputs and provenance

All executable content came from the user's existing installations or disc
images. No game download, installation modification, or game execution was
needed. Windows files were copied read-only from the user's remote installation.
Credentials are not recorded here or in the audit files.

| Candidate | Identifying metadata | Inspected main executable |
| --- | --- | --- |
| Windows GOG | GOG game ID `1207659134`, English installation | `ALPHA2.EXE`, 5,586,432 bytes |
| Windows original CD | User-identified CD release; installed copy and mounted `ALPHA2` volume | `ALPHA2.EXE`, identical to GOG |
| PlayStation Japan | `SLPS_004.15`, disc volume `STREET_FIGHTER_ZERO2` | `SLPS_004.15`, 616,448 bytes |
| Saturn Japan | `T-1212G V1.001`, header date `19960818` | `0.BIN`, 279,556 bytes |

SHA-256 of the Windows, PS1, and Saturn main executables respectively. Both
Windows installations and the executable on the mounted CD share the first hash:

```text
a0df7fbd121814927a4a751b7f1f98d02da8ec2db625a739a2761fc9f458760b
0a522fd4791edfe95150edea057ca7113b8d0e93ed085be6854c042012c4a2a2
7f5c1421df9bc3e6fed2187b60113a2bf829b2c738f56dde3a064d0084ec36b0
```

SHA-256 of the complete PS1 and Saturn raw disc images respectively:

```text
397f761609f9b2cb82150a533d5ab820a797852a0703346c633df35ff526e255
69f24dfc56db3c5a8b6d08fdc0cd576672033dad02d08e27523ff5bbbecc41d0
```

The GOG manifest identifies its installer as `galaxy_sfa2_2.1.0.13.exe`. That is
package metadata, not proof of an original game revision. Likewise, the PE
timestamp is a header field, not independently authenticated build history.

## Windows evidence

### Executable and debug metadata

- Native 32-bit x86 PE, image base `0x00400000`, entry `0x0056e000`.
- PE linker version field is `5.0`; timestamp field decodes to
  `1998-04-23 11:29:30 UTC`.
- Five conventional sections: code, read-only data, writable data, imports,
  and resources. Code virtual size is 1,546,557 bytes.
- COFF symbol-table pointer and symbol count are both zero. No export table.
- The debug directory contains MISC, FPO, and CodeView records. The 47-byte
  CodeView record is an `NB10` external-PDB reference, not an embedded database
  of source names and types.
- The referenced developer path is `E:\NewStzero2\DEBUG\ZERO2'.pdb`, with
  signature `353f2374` and age `1`. This is a potential future identification
  key, not a file recovered from that developer machine.
- Recursive inspection of 2,285 installed files found no `.pdb`, `.dbg`, `.sym`,
  `.map`, `.obj`, `.lib`, `.c`, `.cpp`, or `.asm` candidates. This says nothing
  about other releases or external archives.
- Ghidra explicitly reported that it could not locate the PDB. Names inferred
  by its analyzers or imported from Windows APIs are not original gameplay
  symbols.

The field interpretations follow Microsoft's
[PE format specification](https://learn.microsoft.com/en-us/windows/win32/debug/pe-format).
Neither a debug-directory entry nor a pathname proves that full symbols ship
with an executable.

### Evidence beyond Windows support libraries

The executable contains Visual C++ debug-runtime diagnostics. Those alone
would prove little about gameplay, so the audit also examined game-like native
functions and data:

- In the sampled address interval `0x00410000` through `0x0053ffff`, all 8,068
  function starts identified by Ghidra use the same conventional x86 frame
  prologue. This is a heuristic count, not independently established function
  coverage or proof of C origin.
- Native routines sampled across that interval perform structured state
  dispatch, packed-field updates, table indexing, and fixed-point-like motion
  changes. Their stack temporaries, repeated reloads, argument handling, and
  control flow are consistent with unoptimized compiler output.
- Diagnostic strings preserve game-specific identifiers, including
  `PlayerWork`, `pl_type`, and `pl_init_2`. Several diagnostic strings have no
  direct code references, so their presence is not proof of an active debug
  menu or callable function of that name.
- The Windows message loop calls an internal frame-update routine. The
  inspected frame logic is native x86 code with game-state accesses, rather
  than merely a thin platform wrapper being mistaken for the game.

**Inference:** a substantial native C/C++ port is strongly supported. Visual
C++ 5-era tools are a reasonable first compiler hypothesis. It is not yet
established that every gameplay module used that compiler, that every module
was C rather than C++, or that no assembly/transliteration remains.

Useful audit landmarks, with roles still inferred rather than recovered names:

| Address | Static observation |
| --- | --- |
| `0x0055f240` | Windows message loop and animation/character diagnostic output |
| `0x005477ad` | Internal frame-update candidate called from that loop |
| `0x0041002f` | Object-state flags and direction-dependent coordinate update |
| `0x00480012` | Chained condition/action dispatch candidate |
| `0x004e018c` | Small packed-table arithmetic helper, possible compiler pilot |

None of these inferred roles has been confirmed with gameplay breakpoints.

### Original CD comparison

The user's original-CD installation was located through its Windows uninstall
registration, separate from GOG. The CD was already mounted as a read-only CDFS
volume named `ALPHA2`; no mounting, installation, or launch was performed.

- Compared the installed CD executable and the executable directly on the
  mounted disc with the installed GOG executable. Both native Windows `fc /b`
  comparisons reported no differences. All three SHA-256 hashes also match the
  previously copied and analyzed GOG executable.
- Recursively inventoried 2,273 files in the CD installation and 2,676 on the
  mounted disc, including hidden files. Both enumerations reported zero errors.
- Neither tree contained files with the searched symbol/source extensions
  (`.pdb`, `.dbg`, `.sym`, `.map`, `.obj`, `.lib`, `.c`, `.cpp`, `.asm`), or common
  archive extensions (`.cab`, `.zip`, `.7z`, `.rar`). This is a filename-based
  search, not proof that every asset contains no embedded metadata.
- The disc's root `Install.Dat` begins with a plain-text installation file list,
  not a separate game executable. Installer and DirectX support programs were
  not treated as alternative gameplay builds.

**Result:** this CD copy adds provenance, but not a different game build or an
observed debug-symbol advantage. Its game executable has exactly the same code,
debug-directory records, and missing external-PDB reference already inspected
in GOG. No second Ghidra analysis is needed for identical input bytes.

This finding applies to the user's available copies, not every CD pressing or
regional release. It establishes executable equality, not equality of all
assets or launch behavior, which may depend on platform wrappers and settings.

## PlayStation evidence

- Inventoried 260 files in the ISO filesystem. No obvious symbol sidecar was
  identified by filename.
- The main program is a standard PS-X EXE with a 2,048-byte header and a
  614,400-byte load payload. There is no trailing region beyond that payload.
- Header load address is `0x80118900`; entry point is `0x80118908`. The boot
  configuration identifies the same executable.
- Readable C source filenames found in the main executable belong to Sony SDK
  components. They do not establish C-origin gameplay or recovered game symbols.
- Main-program disassembly and decompilation are possible. Ghidra's generic
  MIPS language is a superset, not a strict PS1 R3000 decoder; the startup
  decompilation also emitted a bad-instruction warning. Output is exploratory.
- Character/stage PAC containers and auxiliary movie executables exist. The
  main executable alone is not a complete inventory of potential game code.
  Overlay extraction and load mapping remain unfinished.

The executable layout was checked against the original reverse-engineering
[PSX executable format documentation](https://psx-spx.consoledev.net/ps1/cdr/cdromfileformats/executables/).

Conclusion: viable further investigation, but this copy did not provide an
identified compiler or source-symbol advantage over Windows.

## Saturn evidence

- Inventoried 515 files. No obvious symbol database was identified.
- `0.BIN` contains startup, native SH-2 code, and Sega library identifiers.
  Selected `.Z` files, including `PLAYER.Z` and `PL000.Z`, begin with plausible
  raw SH-2 routines. Their extension must not be assumed to mean compression.
- Files named `ENDING17.MAP` and `NOWLOAD.MAP` contain binary data, not an
  observed textual linker symbol map. A `.MAP` suffix is not sufficient evidence.
- The disc IP header specifies first-read destination `0x06008000`. Its
  application initializer transfers control to `0x0600d000`, at offset
  `0x5000` in `0.BIN`; that startup stub branches to `0x0600e5d0`.
- Ghidra's installed strict SH-2 language can analyze this code. Overlay load
  addresses, call interfaces, and compiler identity remain unverified.

Boot fields were checked against Sega's
[disc format and boot-system documentation](https://www.infochunk.com/saturn/segahtml_en/prgg/sofg/disc/hon/p04_10.htm).
An initial exploratory import used an incorrectly inferred base address. That
output is superseded; only the corrected import is evidence for this report.

Conclusion: another viable native-code candidate, but currently with more
unresolved loading/toolchain work and no observed symbol advantage.

## Historical verification and reproducibility

This report predates the repository split. Its artifact paths below are
historical provenance, not current commands. Current PS1 and Windows locations
are listed in the [migration record](../docs/migration.md). Saturn binaries and
projects were not migrated, only small comparison metadata.

Private working files live under ignored `local/target-audit-20261002/`:

- `disc_audit.py`, per-disc `inventory.json`, and `extracted.json` record input
  hashes, sector layout, file extents, and extracted-file hashes.
- A separate extraction with macOS `bsdtar`, after stripping raw-sector
  framing, reproduced the PS1 executable, boot configuration, and one character
  PAC, plus Saturn `0.BIN` and `PL000.Z`, byte-for-byte. Results are in
  `extraction-crosscheck.json`.
- `windows/pe-audit.json` records parsed PE/debug metadata. Apple's LLVM
  `objdump --private-headers` independently checked the main PE fields.
- `windows-cd/comparison.json` records the CD paths, file counts, extension
  search scope, executable hashes, and direct binary-comparison results.
- Ghidra 12.1.4 headless projects, scripts, logs, function inventories, and
  unreviewed decompiler output are local only. Java 25 was scoped to those
  processes. Windows was imported as PE; PS1 as raw little-endian MIPS payload;
  Saturn as raw big-endian SH-2 with the corrected load address.
- Console image hashes were checked, and the copied Windows executable matched
  the remote hash. The remote executable was rehashed after inspection.

No executable, asset, extracted game code, generated pseudocode, or credentials
are included in this document or added to version control. No rebuild,
byte-matching C test, runtime equivalence test, or decompilation percentage is
claimed. Ghidra finding functions is not decompilation progress.

## Original proposed next pilot (historical)

Keep CPS2's existing baseline and delivery contract unchanged while testing:

1. Freeze the Windows executable hash and identify a legally available matching
   compiler candidate. Confirm its exact version before tuning code generation.
2. Select a small native game helper with a bounded contract, recover its types,
   and attempt a C rebuild. Compare its machine code with the executable,
   distinguishing relocation/address fixups from instruction mismatches.
3. Differential-test boundary cases against execution of the original helper.
   A readable but nonmatching candidate can still be useful, but must be reported
   as behavioral evidence rather than an exact match.
4. Validate one game-state routine dynamically, linking the static code to an
   observable gameplay event. Do not infer semantic correctness from plausible
   pseudocode alone.
5. Use these results to decide whether Windows becomes a separate first game
   target or remains a reference for CPS2. Either choice is explicit: Windows
   x86 work is not implementation progress on the 68000 backend.

Finding a matching PDB would improve the project substantially, but the next
pilot does not depend on locating one. No public prototype, other region, PS2
collection, or modern collection was binary-audited in this investigation.
