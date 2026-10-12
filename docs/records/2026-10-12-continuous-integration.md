## Continuous integration behind feature flags (2026-10-12)

The owner clarifies that this project practices CI/CD: source must remain
integrated throughout development, including unfinished features. Feature
flags preserve original behavior while new behavior is work in progress.
Commit and integrate small, buildable, scoped-tested increments frequently;
do not maintain long-lived branches or defer all integration until completion.

Experimental switches belong in the shared settings file under [experimental]
and default off. Use the normal build and one settings model for startup and
the future UI. Keep source in tracked domain locations; private inputs and
generated artifacts remain ignored in stable purpose locations.

AGENTS.md now records this rule and replaces the conflicting requirement to
create a branch/PR for every change. Work on main by default; branches/PRs are
only created when explicitly requested and remain short-lived. Existing
publication/provenance safeguards and scoped validation still apply.

Correction to prior workflow: indefinitely keeping implementation in private
trial trees while waiting for feature completeness is not the accepted model.
