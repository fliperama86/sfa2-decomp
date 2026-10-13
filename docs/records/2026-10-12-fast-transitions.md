## Fast transitions after a load, under fast_loads (2026-10-12)

The owner asked to see how far the loading and black screens can be removed, starting with the one after the opponent-path screen of the
arcade mode. A private run on the Mac build with a temporary event log (not kept) timed that transition: 1.37 s from the last full frame
to the first frame of the fight, of which about 1.3 s black. Where it went, and what the setting `fast_loads` now does about it:

- Four loads of 170 to 250 ms each, almost all of it the host sleeping inside polls that the game's loading code makes (TestEvent and
  others call `port_tick`, which slept up to 2 ms a call when the game was ahead of the clock). While a fast load loop runs, `port_tick`
  does not sleep (`port_clock_hurry`, nesting): the loads take 3 to 6 ms each.
- One `StoreImage` of 160 to 600 ms. It was not the readback: the game makes over a thousand `LoadImage` calls just before it, PsyZ's SDL GPU
  backend cycled its 2 MB upload transfer buffer for each one, and the driver allocated a new buffer for each (over a tenth of a millisecond
  a call), all of it paid when the first readback waited for the GPU. `port/psyz.patch` gains `Psyz_Sfa2SetBatchedUploads`: with it on, the
  uploads of one command buffer go one after the other into the one buffer and only the first cycles it. The port turns it on with the
  setting. The StoreImage then took 12 ms.
- Eight to fourteen frames after the last load (the end of the loading screen's script, the music's start, the display on, the next module),
  one every 1/60 s, with the display showing only about 30 packets. From the loading screen's fade-out state until a frame with 150 packets
  or more is drawn (`port_clock_frame_drawn`, called by the list walker), or at most 40 frames, `port_tick` makes a vblank that is not due
  yet due: the clock's start moves back a frame (`port_clock_blank_start`). The game still takes every frame, counted as before.

The same transition timed 0.25 s with these (the last full frame to the first fight frame), from 1.37 s. A second transition (the title to the
menu) had the same frames after its load and is covered by the same rule. Not timed after the final form of the blank rule: the owner's play.

Checked: `test_hostkernel_start.py` (the clock's three hooks on the real kernel.c; three mutants each fail their cases),
`test_hostloadwait.py` (the hooks of the loop and of the loading screen's states), `test_psyzbuild.py` (the patch applies at 17 places),
`test_video.py` (its list of patched files lacked `sdl3_audio.c` since the volume option; fixed) and stand-ins added to the controls that link
`gpu.c` or `main.c`. Not run: the launch, modules, programs and GPU controls (they need a Windows cross compiler and a Windows run), and
`test_ui_layout.py`, whose real-seam case aborts in this Mac's environment on main as well (a 64-bit `unsigned long`).
Unknown: whether the frames of a blank run, taken faster, change anything the game later depends on (the music's start is among them), and
whether the batched uploads change a picture (the owner looked at one transition).
