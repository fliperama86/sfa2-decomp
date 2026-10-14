## Owner testing before a feature PR (2026-10-14)

The owner corrected the intro workflow: before a pull request is opened, they
must be able to test the feature. Opening PR369 for a verified but disconnected
sequence reader was premature. Synthetic comparisons establish that primitive's
behavior; they do not provide a testable intro.

Close PR369 without merging, retaining its commits and evidence on the task
branch. The existing code-review approval covers the inactive primitive and
does not override this newer workflow correction. No code or evidence is discarded.

The next intro review handoff must include a runnable normal-port build, exact
launch/input instructions and explicit scope/limitations, before a PR is opened.
It need not wait for the entire intro: a clearly bounded visible scene is a
valid increment. Use the normal shared experimental setting, default off, not
a separate preview executable. Keep all PS1 reconstruction files untouched.
This does not add a game-boot gate to already byte-identical PS1 code.
