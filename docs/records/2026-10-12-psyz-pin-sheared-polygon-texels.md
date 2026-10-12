## PsyZ pinned to its change #196, slanted-polygon texels (2026-10-12)

The owner's pull request to PsyZ, #196 ("gpu: keep a slanted polygon's texels
inside its own UVs"), fixes wrong-colored pixels along one edge of tiles that are
drawn with a slanted edge (the stage floor): the sample was taken half a texel
back from the pixel centre, so on a slanted edge it could lie in the neighbouring
tile. The vertex record now carries the smallest u and v of the corners and the
fragment shader takes no texel below them. The pin of `port/external/psyz` moves
from `6d01339a` to the change's commit, `4c68cab4`, one commit on top of the old
pin; the change is not merged upstream yet, and the commit is fetched through its
pull request ref (`git submodule update --init` does it by the hash). When it is
merged the pin moves to the commit PsyZ's main has.

`port/psyz.patch` needed one hunk's context changed (the change adds code just
above `FixupFlipUV`, where the patch's HUD pixel-grid block replaces the call);
the lines it adds are the same, its header names the new commit, and
`psyzbuild.py` applies it at 10 places. `test_psyzbuild.py` (header of the real
patch) follows the new commit and passes. PsyZ builds for macOS (about 30 s).
