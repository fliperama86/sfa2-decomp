# Isolated agent worktrees

Owner-approved layout, 2026-10-12. This page documents the setup; it does not
claim that the folders, launch guards or a bare repository have been provisioned.

## Layout

The worktrees live beside the integration checkout, not inside it:

```text
$HOME/Projects/
├── sfa2-decomp/                 # Integration checkout, main
└── sfa2-decomp-worktrees/
    ├── worker-1/                # One worker, one task branch
    ├── worker-2/                # Another worker, another task branch
    └── review/                  # Reviewer, detached at the PR head
```

These are stable folders reused across tasks, not new folders for every PR or
experiment. A worker slot belongs to one active agent at a time. Additional
slots require an explicit assignment, not an agent choosing somebody else's
folder. The project location may differ on another machine; the sibling layout
and ownership rules stay the same. No personal absolute path belongs in Git.

A bare hub is optional. If adopted, it lives beside these folders as
`$HOME/Projects/sfa2-decomp.git`, and the integration checkout and agent
worktrees attach to it. A bare repository has no source checkout for an agent
to work in. Do not convert or move the existing repository merely to follow
this page: that migration needs its own inventory and preservation steps.

## Ownership and branches

- Only the assigned integrator updates `main`, merges approved PRs or performs
  an owner-authorized direct push. Workers do not edit the integration checkout.
- Each worker starts in its assigned worktree and uses a short-lived,
  product-named task branch. Reusing a folder does not mean keeping a permanent
  feature branch. Source and WIP remain tracked, with the existing default-off
  feature flags and small-PR rules.
- The reviewer fetches and pins the actual PR head and base, then reviews in
  `review/` with a detached HEAD. Evidence names that exact commit. A changed
  PR head requires reading the delta and checking which evidence still applies.
- Never switch, reset, clean, stash, rebase, delete or edit another agent's
  worktree. Never use `--force` to check out a branch already owned elsewhere.
  Worktrees still share Git objects and refs; they are not independent clones
  or a security boundary. Do not rewrite another agent's branch or refs.
- Before reassigning a slot, let its agent and jobs finish, preserve pending
  tracked and ignored work, and check its status. Never discard work to make a
  folder reusable. Coordinate before retiring or removing any existing tree.

Every agent reads `AGENTS.md` and the startup documents from its own checkout.
The active repository rule includes these assigned worktrees of SFA2, not the
unrelated old 68k project. All Git and build commands use the assigned root.

## Launch guard

An agent's initial working directory must be its assigned worktree. A future
launcher must verify the resolved Git top-level path against that assignment
and refuse a mismatch before any write or branch switch. Until that launcher
exists, perform the same check at session start and before writes:

```sh
pwd -P
git rev-parse --show-toplevel
git status --short --branch
```

Compare the resolved paths with the assigned slot and verify the expected
branch or detached commit. A checkout silently changed by another session is
a refusal, not a reason to continue on whichever branch is now present.

## Inputs and build products

Worktree isolation also applies to generated files, not just Git's index:

- Each assigned worktree owns its canonical build locations, such as
  `ps1/build/` and the target-specific directories already used inside
  `port/build/`. Reuse them across tasks. This is the worktree-isolation
  exception to the one-build-directory rule, not permission for extra preview,
  fixture, sanitizer or per-run build trees within a worktree.
- Do not symlink or share writable build directories, generated headers,
  tables, copied dependency sources or test scratch outputs between agents.
  Serialize jobs that write the same canonical output within one worktree.
- Private disc inputs, original archives, pinned toolchain installations and
  unchanged SDK inputs may be stored once in their existing private locations
  and exposed read-only. Stage only the paths the actual build configuration
  names. Do not link an entire `local/` directory that also holds writable
  outputs. Ignored inputs do not arrive with a Git worktree automatically.
- Share a writable cache only after its keys, locking and atomic publication
  have been shown safe for concurrent use. Otherwise keep it local. Do not
  assume that every existing cache supports multiple writers.
- Private inputs and generated evidence remain ignored. The scoped-check,
  unchanged-input reuse and publication rules of [efficient checks](efficiency.md)
  remain in force. A new worktree is not itself a reason to repeat a valid run.

The existing frozen-head workflow can stage configured prerequisites from a
separate input repository with `--data-root`; that is selective input staging,
not authorization to share its output folders. Its implementation and existing
report layout are unchanged by this documentation.

## PsyZ and other submodules

PsyZ has its own pinned checkout, including its SDL dependency. Do not share
one writable submodule checkout between agents. Git documents incomplete
submodule support with multiple worktrees, so initialization, pinned updates
and cleanup must be validated in an isolated setup before assigning port work
to the new slots. A bare hub does not remove that limitation. If the setup
cannot safely provide independent dependency checkouts, use separate clones
for the affected slots rather than forcing Git's worktree safeguards.
See the [Git worktree documentation](https://git-scm.com/docs/git-worktree#_bugs).

## Provisioning and review handoff

Provisioning is a separate step from this documentation change:

1. Inventory existing worktrees, active sessions, branches, pending changes,
   ignored private inputs and canonical outputs. Preserve them in place.
2. Assign the worker and reviewer slots, create missing worktrees without
   forcing branch ownership, and validate the submodule setup and input access.
3. Restart or redirect agent sessions explicitly into their assigned folders,
   verify their roots, and allow only the integrator to operate in the main
   checkout. Do not silently relocate a running agent.
4. Keep each task's branch, PR, owned paths and evidence commit in its handoff.
   Do not transfer context by letting another agent inspect a moving checkout.

Reviews end with an explicit plain PR comment: `**APPROVE** SHA` or
`**REQUEST CHANGES** SHA`, followed by scoped evidence and remaining findings.
Review code first: reading is sufficient when correctness is clear. Run a
scoped check only to resolve a specific uncertainty, not as a routine gate;
reuse existing verified evidence. Author validation requirements still apply.
Passing CI or a COMMENT-only report saying "no blockers" is not the reviewer
verdict. Normal changes still use reviewed PRs; the owner's direct-main
authorization for publishing this setup is specific to this documentation.
