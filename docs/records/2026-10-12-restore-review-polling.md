## Restore review polling in active memory (2026-10-12)

The owner asks to restore the 60-second PR-comment polling instruction in
active project memory and push the correction. It was inadvertently omitted
while rewriting the workflow summary. AGENTS.md retained the requirement.

Active memory again explicitly requires polling owner comments every 60 seconds
while active. Reviewed PRs, short-lived branches, frequent integration and
default-off settings flags remain unchanged. The correction stays on the
existing review-policy PR branch rather than bypassing review on main.
