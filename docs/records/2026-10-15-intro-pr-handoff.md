## Complete-intro publication handoff (2026-10-15)

After receiving the complete normal Windows build and launch instructions,
the owner explicitly requested committing, pushing and opening a PR, then
reaffirmed review polling every 60 seconds. This authorizes publication, not
automatic merge or an invented claim about the owner's visual/audio findings.
Only owner-account comments are actionable; review approval remains required.

Implementation is at `04466f13`; `42e215ee` records the preference for using
the existing build/run script rather than a manual launch command. This
handoff changes documentation only: the port landing-page summary now describes
the full sequence rather than the old slice, active status records publication
authorization, and three record headings are brought into the required dated
format. Their original measurements and conclusions are unchanged.

The complete-runtime evidence manifest was checked against current source:
all implementation, importer, runtime and test hashes still match; the only
preceding difference is the documented memory preference. Prior scoped tests,
normal build, independent-startup comparisons, resource closure, GPU capture
comparison and silent audio run are reused from
[the runtime record](2026-10-14-intro-full-runtime.md), not repeated for a PR.
No full-game, PS1 matching, or unrelated validation run was added.

Publication inspection covers the branch's complete diff from merged PR370:
only port source, synthetic fixtures/controls, workflow dependency mappings
and documentation are included. No PS1 file, private output, supplied game
asset, binary, key, decompiler artifact, credential or personal machine path
is included. The import-only third-party cipher retains its BSD license,
copyright and provenance; all other new source is project-authored. JSON
fixtures are retained original-computed results for synthetic inputs, not
extracted game assets. Whitespace and record-format checks pass.

The feature remains default off, replaces only the opening program, and
rejects obsolete first-scene packs. Sound remains the explicitly documented
PS1 XA adaptation, not bit-perfect QSound. The rest of arcade attract mode
and the whole-port CPS2-only asset pivot are not included in this PR.
