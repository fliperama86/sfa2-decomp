## Independent agent worktree setup (2026-10-12)

The owner approved stable sibling worker and reviewer folders, with the
existing primary checkout reserved for integration. `docs/agent-worktrees.md`
records the layout, slot ownership, short-lived branches, pinned review heads,
root checks, read-only private input access and isolated canonical build outputs.
A bare repository is optional; submodule isolation needs validation before
provisioning port workers. No worktree, session, private input or build directory
was created, moved or removed by this documentation change.

The owner's earlier correction is recorded too: PR reviews give explicit
`**APPROVE** SHA` or `**REQUEST CHANGES** SHA` plain-comment verdicts, as prior
PRs did. COMMENT-only reports of no blockers were not that verdict.

The owner explicitly requested that this documentation be reviewed and pushed
directly to main. That exception does not waive the normal PR rule for future
implementation. Validation is a scoped documentation, link, record-format and
publication review; no compiler, game or unrelated test suite is required.
