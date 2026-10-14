## Sharp-bilinear window scaling (2026-10-14)

The owner asked whether PsyZ has sharp-bilinear scaling, then "let's try".
PsyZ at the pin and the four upstream commits after it draw the picture to the
window with nearest sampling only, in both backends.

The change, behind the new default-off setting `sharp_bilinear` (command line
`--sharp-bilinear`, `--nearest-scaling`): the SDL_GPU backend, through
`port/psyz.patch`, first enlarges the picture by whole factors with nearest
sampling into a texture of its own, then stretches that texture to its place
in the window with linear sampling. The factors are the largest whole factor of
each side that fits the place, computed by `port_video_sharp_fit` in the shared
policy header; for the 384x224 canvas in a 1280x960 window that is 1152x896,
then linear to 1280x960. When the place is a whole multiple of the picture on
both sides, and with the setting off, presentation is the one nearest blit as
before. The texture is made again when the size changes and freed with the
backend's other textures. The OpenGL backend has a no-op setter, and the port
refuses the setting without the patched backend, as for the other options. The
backend's API marker is now 3, so a library built from the older patch is
refused before the game is compiled.

What ran, on the branch's tree: `test_video.py`, `test_ui_layout.py` and
`test_settings.py` pass; the patch applies at 19 places to a copy of the pinned
source; the library and the Windows program build (the program in 21 seconds)
and the program reports the setting with `--show-settings`. One hidden, silent
start with the setting on ran 45 seconds through the logo programs into the
game; the offscreen driver has no swapchain there, so that start did not run
the new presentation path. Not run: anything that reads the window's pixels.
How it looks is the owner's to judge.
