# AI workflow: implementation contract

The owner requested all five improvements on 2026-10-09: bounded active
context, one-command reviews, a linked function ledger, dependency-aware
validation reuse, and small work packages with explicit acceptance criteria.
This workflow changes no game source and adds no gameplay gate.

## Interfaces

`python tools/ai_workflow/workflow.py` provides `doctor`, `context`, `ledger`,
`review`, and `task`. It uses Python 3.11+ and the existing project tools.
Generated reports, cache entries, frozen checkouts and binary-dependent
results stay under ignored `local/ai-workflow/` by default.

- `context`: read short active state, or retrieve matching sections of the
  historical memory with bounded output. History remains byte-preserved in
  the same documentation directory, so relative links retain their meaning.
- `doctor`: check tool availability and the actual configuration's local
  private input paths. It never installs software, changes inputs or starts
  a game. Missing prerequisites are refusals, not passes.
- `ledger`: extend the existing coverage inventory and configuration, rather
  than maintain a second hand-edited function list. Identify a function by
  image/content and address. Link exact owners, nonmatching source and
  contracts, documentation/evidence, and missing-C blockers. A declared owner
  is not a new byte-comparison result, and a contract file alone never means
  tested nonmatching C. Second placements and raw/assembly/data exceptions
  remain distinct. No original bytes enter the ledger.
- `review --base REF --head REF` (or `--pr NUMBER`): freeze the head, resolve
  private inputs from the supplied local input repository, and select checks
  from the diff. Matching-input changes use the authoritative matching build;
  nonmatching additions run differential cases on two seeds, actual negative
  controls, and original write audits. A changed override of the port
  (`port/overrides/NAME.c` or `NAME.py`) runs the same three on its
  contract, and a change that broadens (a shared header, the harness)
  runs them on every override. Port/tool changes select their controls.
  A plan-only mode explains commands without claiming that they ran. Results
  record the exact commit, scope, commands, exit status and evidence hashes.
  Passing checks never automatically approve a PR.
- `task`: write a small work-package template with scope, owned paths, pinned
  contract, acceptance commands, evidence and unresolved decisions. A task
  with missing sections fails validation. Decision-bearing forks must stop
  for the owner, not be guessed by an implementation worker.

## Validation reuse

Cache successful CHECK RESULTS, not verdicts. A key includes the selected
check and its arguments, all tracked compiler/test inputs in its dependency
scope, ignored SDK headers where needed, real baseline/archive/compiler bytes,
external compiler/assembler/wrapper identities, Python version and package
versions. For matching checks, the existing matching build also rechecks its
pins and its own object cache. Unresolved dependencies disable reuse: they do
not yield a weakened key. Evidence must exist and hash correctly. Failed,
interrupted, incomplete, malformed or stale results are never reused.
A forced-fresh switch reruns checks and negative controls. Cached results
name their originating commit and remain distinguishable from fresh runs.

Selection is deliberately conservative: shared headers, bindings,
configuration or validation-tool changes broaden the check set. Unknown
code/build-input changes refuse an automatic pass and require explicit scope.
No commit hash alone is a dependency key; no unchanged source alone proves
unchanged behavior after a header, compiler, contract or binding change.

## Acceptance

Standard-library unit tests cover selection, archive context retrieval,
ledger categories, config-based staging, command failure propagation,
cache hits and invalidation (source, header, binding, compiler, baseline,
contract, arguments), missing/tampered evidence and failed/partial entries.
An actual repository ledger must match the existing coverage-map totals.
A real frozen-head review must run successfully with available private inputs,
then repeat with verified cached evidence, then reject corrupted cached evidence and run fresh.
Documentation-only work must not require unavailable game inputs.

## Everyday commands

From the repository root, use the environment containing `requirements.txt`'s
packages. Python 3.11 or newer is required. No tools are installed by this CLI.

```sh
python tools/ai_workflow/workflow.py doctor
python tools/ai_workflow/workflow.py context --query "register allocation" --limit 80
python tools/ai_workflow/workflow.py ledger
python tools/ai_workflow/workflow.py task --name "one bounded change" > local/task.md
python tools/ai_workflow/workflow.py task --check local/task.md
```

Fill the generated task with actual paths, a pinned contract and commands before
using it. Task validation is structural, not proof of acceptance. Regenerate the
history index after adding historical evidence with `context --reindex`.
The original archived memory and goals remain verbatim; links stay in `docs/`.

For a review, supply the local repository that contains the private prerequisites
if it differs from the shared primary checkout. Supply a working 32-bit native
compiler when game source or port inputs change:

```sh
python tools/ai_workflow/workflow.py review --pr 123 --plan --cc "$CC32"
python tools/ai_workflow/workflow.py review --pr 123 --data-root "$INPUT_REPO" --cc "$CC32"
python tools/ai_workflow/workflow.py review --base "$BASE" --head "$HEAD" --cc "$CC32" --fresh
python tools/ai_workflow/workflow.py ledger --evidence local/ai-workflow/reports/REPORT/report.json
```

`--pr` reads GitHub and fetches the immutable base/head commits; it does not
comment, approve or merge. Offline base/head refs must already exist locally.
`--plan` creates a frozen checkout and lists commands but never stages private
inputs or runs checks. Defaults are 2,000 cases, seeds 1 and 7. Lower case counts
are smoke tests, not a claim to reproduce the documented 2,000-case evidence.
Native/PsyZ controls require their prerequisites explicitly, never silent skips.
Timeouts kill the entire child process group. Failure and interruption leave a
failed/incomplete report and return nonzero; the first failed check stops the run.

## Evidence and limits

Reports name the exact Git head, selected scope, real child status, log hash and
source manifest. Reused checks additionally name their original head. A ledger
accepts only complete, current-head evidence with intact logs and an unchanged
full source-dependency manifest. Two distinct differential seeds, both original
write audits and an actually tripped control are required for tested-nonmatching
status. Exact owners still need the authoritative matching build. Report passes
are never GitHub approvals.

Cache closure is deliberately conservative on Linux: tracked scoped inputs,
real private compiler/baseline/archive/SDK files, pinned wrapper export, tool and
shared-library hashes, Python standard library/packages/startup configuration,
arguments and an environment digest. Loader/import overrides or unresolved
Python `.pth` startup hooks disable reuse. Native/PsyZ result reuse is disabled
because its full external/platform closure is not yet modeled; those jobs run
fresh. The existing matching tool keeps its own pin checks and object cache.
Git metadata alone and source-only hashes are never sufficient cache keys.

Canonical map totals are retained for compatibility. Its current second-link
classification includes 14 function rows whose units are excluded from that
placement. The ledger marks those rows raw with missing C and a discrepancy
label instead of inventing applicable owners. Inventory boundaries are estimates
and may span several declarations; `owners` preserves all overlapping owners.
Data-only rows are excluded using the canonical inventory classifier.

The context documents are entry points, not replacements for historical evidence
or a new declaration of whole-game completion. Unknown build/code changes refuse
automatic scope. No CI, private baseline or toolchain bytes are committed here.

For optional Python extension modules that cannot import because a system
library is absent, the dependency key records that absence and the available
libraries. Installing the missing library changes the key. Required executable
and selected package dependencies must resolve; they do not get this treatment.
Tracked submodule links retain their pinned commit and available content identity,
not a fabricated regular-file hash. Native/PsyZ checks still run fresh.

Second-placement nonmatching rows may link the first placement's candidate,
but never inherit tested status from its contract: a separate original-image
contract is required there. Matching reviews share the authoritative builder's
own dependency-keyed object cache across frozen checkouts; they still perform
its relinking, full claimed-output comparisons and controls unless an intact
whole-check result is independently reusable.

## Implementation validation

The workflow's 27 standard-library controls pass, including failed/zero-case
jobs, source/header/binding/compiler/baseline/contract/argument invalidation,
missing or tampered logs/manifests, subprocess timeout, metadata categories,
second-placement limits and archive identity. The ledger's 13,072 rows and
aggregate totals equal the existing canonical map; it separately flags missing
applicable second-link ownership.

A private, isolated trial uses the real repository at the merged character-C
baseline plus one comment-only change to one nonmatching C file. Its original
function and supplied disc inputs are unchanged. The frozen review runs 2,000
differential cases on each of seeds 1 and 7, both original write audits, the
actual negative control and the complete native link. A repeat reuses the five
intact nonmatching results and reruns native validation. Corrupting one cached
log causes that check to run fresh, not be accepted; the other four remain
reusable. The trial establishes this workflow at one-function scope, not a new
matching build, full-game result or gameplay observation. Private reports retain
the trial's exact commit, dependency manifests, origin heads and log hashes.

The prior memory (8,082 lines) and goals (554 lines) are byte-identical to the
implementation branch's parent records. The active memory is under 120 lines.
No PS1 or port source is changed by this implementation. New published tools
and fixtures are written here; no third-party sources, private machine paths,
credentials, binaries, assets or SDK headers enter the commit.
