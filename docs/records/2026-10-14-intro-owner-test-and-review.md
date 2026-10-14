## Owner-tested intro slice ready for review (2026-10-14)

The owner tested the normal Windows port built from source at 1bb04c0b, with
arcade_intro enabled and the prepared private CPS2 scene pack. The owner's
response was that it looked pretty good. After being told this slice uses
CPS2-derived native C and data, not ported Saturn code, the owner authorized
opening a small PR for the tested, default-off slice.

This meets the test-before-PR requirement that closed PR369. It is not a
formal approval to merge the new PR, a whole-intro fidelity claim, or approval
of a CPS2-only whole-port pivot. PR369 remains closed; its primitive-reader
review and scoped evidence are reused, not silently treated as approval of
the newly connected runtime.

The retained source manifest, executable digest, pack digest and GPU readback
report still match the tested implementation. No unchanged build or tests
were rerun for this documentation/review handoff. Existing authored evidence
is in the first-scene and sequence records. All assets and binary-dependent
artifacts remain private; nothing under ps1/ changed.

After review and merge, the next bounded implementation is the first scene's
missing fade/flash transitions and moving embers, recovered from the CPS2
controller. Sound follows in its own scoped increment. Keep the shared flag
default off and provide the owner another runnable build before its PR.
