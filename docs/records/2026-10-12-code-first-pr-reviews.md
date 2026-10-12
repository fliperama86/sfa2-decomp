## Code-first PR reviews (2026-10-12)

The owner corrected the review loop: reading the code is sufficient when the
reviewer can establish that it is correct. Run a scoped test only when needed
to resolve a specific uncertainty, not as a routine gate. Reuse existing
verified evidence and preserve the efficient-check rules for any necessary run.

The review of PR #357 had already started additional helper/settings tests
after a concrete compile failure was reproduced. Those additional runs were
unnecessary as a default review step; this correction applies going forward.

`AGENTS.md`, the review section of `docs/efficiency.md`, the AI-workflow page,
the agent-worktree page and active memory now state the corrected default.
Author acceptance and exact/nonmatching evidence requirements are unchanged;
the validation CLI remains available but is not a mandatory PR review step.
The owner explicitly requested this documentation be pushed directly to main.
No additional build or test run was needed for this documentation change.
