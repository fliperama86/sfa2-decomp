# Active goals and resumption point

Read [current state](current-state.md), [requirements](requirements.md) and
[active memory](project-memory.md). The previous 554-line goals log is
byte-preserved in [goals-history.md](goals-history.md), including the original
order, completed packages and unresolved questions. It is historical evidence,
not a current count of functions remaining.

## Current owner-directed work

The owner requested the five AI-workflow improvements: bounded active context,
one-command frozen-head validation, a linked function ledger, dependency-aware
result reuse and explicit small task contracts. Acceptance is in
[AI workflow](ai-workflow.md). This work changes no game source.

## Reconstruction lane

Every function of the sweep is exact (see [current state](current-state.md)),
so the work is no longer finding code to match. What is left, from the ledger
and the build configuration:

- Game data: replace the retained tables, text and similar data with
  declared, compared owners (the build declares 52 data ranges today).
- Names: fields (68% named) and data symbols (8%) have the most placeholders.
- Source quality: forms found by a permutation search, assembly and SDK
  exceptions, types and comments that make the code readable.
- Delivery: a fresh clone cannot build yet; the import of the user's own disc
  files and the compiler needs instructions and a manifest.

The source is sorted by subsystem, character, stage and screen in
[`ps1/src/`](../ps1/src/README.md); a function renamed with
`ps1/tools/renamefuncs.py` has its file renamed with it.

SDK completion still has owner decisions open concerning widened pointer
arithmetic, volatile-only stores and assembler delay instructions. Do not choose
those architectural exceptions in a grind task. The struct-copy question was
resolved through declaration evidence, as the preserved history records.

## Port lane

Follow [port decisions and state](../port/README.md). Implement missing library
behavior and missing C under explicit contracts, with scoped differential
checks. Missing C stops with its name. No original-code interpreter, supplied
assets in Git or changes to PS1 source for the port before it runs.

Current owner-directed intro work: the default-off Gouki/fire scene slice has
been tested by the owner and is ready for review (source at 1bb04c0b). After
its approved merge, recover the first scene's missing fades/flashes and moving
embers from CPS2 code, as bounded port-only work; sound follows separately.
Provide another runnable owner-test build before that increment's PR. The
whole-port CPS2-only asset pivot remains undecided. See [intro scope](../port/intro/README.md).

## Small work packages

For each next item, fill [the task template](tasks/template.md): owned paths,
pinned contract, excluded work, exact acceptance commands, negative controls,
evidence destination and decisions. Validate it before assigning mechanical
work. On a design-bearing fork, stop and report options.
After owner approval and merge, take the next package without another permission
request. A command's successful exit is not a human approval or whole-game claim.
