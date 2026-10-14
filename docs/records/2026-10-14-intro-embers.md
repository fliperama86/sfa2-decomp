## Native intro embers ready for owner testing (2026-10-14)

PR370 is merged as 23ed5b41. The next source increment, a2e7a1f2, adds moving
embers to the existing 192-update Gouki/fire slice in the normal port. No PR
has been opened: the owner requested another testable build first. Fades,
flashes, warm-up and exact carry-in are deliberately still pending, not
approximated. The existing experimental setting stays off by default and no
file under `ps1/` changed.

The project-authored native C reconstructs the arcade RNG, all three spawn
helpers, particle initialization and wrapping fixed-point motion. The steady
slice integrates only phase 2, with original LIFO allocation, five-slot reserve,
ascending updates and next-update deletion. Its initial seed/interval are
explicit normalizations. The private pack adds original numeric tables,
relocated sequences and small sprite artwork, not executable instructions.
Saturn code is not used. There is no whole-scene equivalence claim.

Version 1 remains accepted with its prior no-particle behavior. Version 2 uses
the existing, pinned offline opcode derivative of the SAME arcade archive to
extract encrypted PC-relative tables. This is not an extra ROM, but its
preparation is still an analysis step: a final user-friendly, automated importer
is not yet delivered. The prepared owner-test pack requires no new extraction.
The old private pack and earlier evidence remain untouched.

### Scoped evidence at a2e7a1f2

Checks ran independently in parallel, with no game boot, audio or GPU window:

- `python port/tools/test_intro_particles.py`: three controls, 41
  original-computed synthetic fixtures, invalid-subtype atomicity, verifier
  guards and four actual rejected C mutations, 0.877 seconds.
- Five selected `test_intro_scene.py` cases: legacy clocks/layer order,
  legacy pack/runtime/skip, new importer layout, ember pool/lifecycle/priority,
  and new pack validation/runtime. Passed in 0.454 seconds. No unchanged GPU,
  graphics lane decoder, settings or original sequence oracle was rerun.
- `verify_particles.py`, seeds 0 and 7, `--cases 8000`: each seed includes
  2,000 cases per reconstructed operation (RNG, spawning, initialization,
  motion), plus the 41 fixtures. Both passed in about 2.7 seconds each.
  Original reads, writes and control-flow were audited, including the explicit
  sequence/render/allocator boundaries. All four negative controls rejected.
  Earlier 2,000-total-case exploratory runs also passed and remain preserved;
  they are not mislabeled as 2,000 cases per operation.
- Normal Windows `hostbuild.py --psyz port/build/psyz-i686-w64-mingw32 --jobs 4`:
  3,982 units compiled, zero failed; executable linked and addresses verified.
  Existing private PsyZ library reused. No PS1 matching build or Mac runtime run.
- Actual supplied-input import produced a 998,866-byte private version-2 pack.
  Native C accepted and rendered all 192 updates, then completed. The base
  payload is byte-identical to version 1; 185 frames contain visible ember
  pixels. A private frame was visually inspected only for usability, not used
  as the implementation specification or proof of original scene equivalence.

Private logs, source manifests, two wide oracle reports, pack and native frame
are in the existing `port/local/intro-asset-probe/`. The normal executable is
`port/build/host/sfa2.exe` in the assigned worker checkout. Select
`intro-embers.dat` with `--intro-data`; other normal launch options are unchanged.
Leave the title idle to reach the slice; Start skips it. The owner has not yet
tested this new build. No live Windows run of this increment is claimed.

Published additions were inspected for provenance/privacy: project-authored C,
tooling, contracts and synthetic fixtures only. No original table, artwork,
binary, decompiler output, third-party cipher or private path is committed.
Existing sequence evidence and the earlier GPU upload/return observations are
reused only for their unchanged scopes. Transitive control dependencies and
native fixture linkage were updated for both new C units.
