## CI/CD retains reviewed pull requests (2026-10-12)

The owner immediately corrects the prior interpretation: CI/CD does not remove
reviews or pull requests. The earlier continuous-integration record preserves
that mistaken interpretation and is superseded here on the review policy.

Use short-lived branches and small reviewed PRs, merging frequently after the
scoped checks pass and the owner approves. Unfinished features may merge behind
default-off settings flags that preserve original behavior. Do not wait for a
whole feature to be finished, and do not use integration cadence to bypass review.

AGENTS.md and active memory restore the existing branch/PR/owner-approval workflow.
The only exception is an explicit instruction from the owner for a particular
change. Stable-purpose directories and private-artifact safeguards are unchanged.
