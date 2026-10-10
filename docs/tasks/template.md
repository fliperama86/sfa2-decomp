# Work package: <short product change>

## Objective
One bounded, independently reviewable result.

## Pinned contract
Spec/decision document, revision and normative behavior. List exclusions.

## Scope
Owned paths and interfaces. State what must remain untouched.

## Acceptance commands
Exact commands, expected exit statuses and required negative controls.
A matching result needs bytes at the correct mapping, not a gameplay gate.

## Evidence
Report destination, exact commit, input/tool identities and remaining limits.
Keep private inputs and generated binary-dependent reports under `local/`.

## Decisions and escalation
Unresolved design decisions: none, or enumerate them before implementation.
STOP and report options on an interface change, material tradeoff, missing
contract or contradiction. Do not decide these in a grind task.
Minor judgment calls may proceed but must be flagged in the report.

## Completion
Acceptance evidence recorded, publication/provenance checked, PR opened.
No completion claim based only on a successful command wrapper.
