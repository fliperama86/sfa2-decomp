# Work package: AI workflow and evidence tooling

## Objective
Implement the owner's five AI-friendly workflow improvements without changing
game source or existing byte-identity acceptance rules.

## Pinned contract
`docs/ai-workflow.md`: context/history separation, config-resolved frozen review,
linked metadata ledger, fail-closed successful-result cache and small contracts.
No formal GitHub reviews, automatic approval, new emulator or gameplay gate.

## Scope
Own `tools/ai_workflow/`, active context documents, preserved history/index,
`AGENTS.md` startup guidance and this task. Leave PS1 and port source, original
inputs, unrelated worktrees and the old 68k project untouched.

## Acceptance commands
```sh
python -m unittest discover -s tools/ai_workflow/tests -v
python tools/ai_workflow/workflow.py ledger
python tools/ai_workflow/workflow.py context --query "matching" --limit 40
python tools/ai_workflow/workflow.py task --check docs/tasks/ai-workflow.md
```
All must exit 0. Ledger totals must equal the canonical map. Preserve archive
bytes and verify their hashes against the parent commit. Run a real frozen
review of an isolated private one-function trial with supplied inputs, then
repeat to verify reuse and tamper an entry to require fresh evidence. This is
a workflow smoke test, not a new whole-game or original gameplay claim.

## Evidence
Ignored `local/ai-workflow/`: exact heads, logs, child exit statuses, dependency
keys, origin of reused results and source manifests. Public documentation
records commands, smoke-test scope and limitations, never private paths/bytes.

## Decisions and escalation
No unresolved design decisions in the workflow contract. STOP and report options
on an interface change, material tradeoff, missing contract or contradiction.
Do not invent new completion gates or decide SDK/port exceptions. Minor calls
must be flagged. Native/PsyZ result reuse stays disabled until closure is known.

## Completion
All acceptance evidence recorded, archives verified, first-publication
privacy/provenance checks completed, PR opened. Owner approval remains required
before merge. Do not self-approve this implementation PR.
