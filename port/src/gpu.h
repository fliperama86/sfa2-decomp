/* The graphics layer's interface: host routines for the graphics library
 * functions the game calls (ResetGraph, DrawOTag, PutDispEnv, the Set*
 * primitive setters, ...), drawn by PsyZ (see gpu.c).
 *
 * How the rest of the runtime uses it:
 *  - domains.c lists port_gpu_library[] (library functions without C, keyed by
 *    the name the build's tables give them), and library.c writes the jumps.
 *  - kernel.c presents the frame through port_gpu_present, once per vblank,
 *    after the vblank handlers.
 *  - debug.c reads the picture back for --dump-vram through port_gpu_read_frame.
 *  - this layer calls port_tick() in DrawSync, the call in which the game
 *    waits for the drawing to end; it calls port_callbacks_reset() for
 *    ResetGraph(0), as PSY-Q's ResetGraph does ResetCallback; and it ends the
 *    program with port_halt(PORT_EXIT_GRAPHICS, ...) for a list or a transfer
 *    it must not follow.
 * Without PORT_HAVE_PSYZ (a build that did not link PsyZ) the table is empty
 * and port_gpu_present does nothing, so every library function of this
 * domain keeps its stop call. hostbuild.py defines PORT_HAVE_PSYZ for gpu.c
 * when it is given --psyz. */
#ifndef PORT_GPU_H
#define PORT_GPU_H

#include "port.h"

extern const struct port_library port_gpu_library[];    /* ends with a null name */

/* Show the frame that the drawing so far has made and pump the window's
 * events. Never calls a callback of PsyZ's own kernel. A closed window
 * (or Escape) ends the program with status 0 after the line
 * `stop: window closed`. Does nothing until the game has used a graphics
 * routine. Declared in port.h too; the same prototype. */
void port_gpu_present(void);

/* The video memory, 1024 x 512 halfwords, into a buffer of the caller's own
 * (not the game's). 0, or -1 (buffer untouched) when the graphics were not used yet
 * or the window is gone. */
int port_gpu_read_frame(unsigned short *pixels);

/* The display enable the layer last sent to PsyZ: 1 after SetDispMask(non-zero), 0 after SetDispMask(0) and after
 * ResetGraph(0) (the hardware reset turns the display off). For the controls and the picture dump. */
int port_gpu_display_enabled(void);

/* The frame buffer, in halfwords. */
#define PORT_GPU_FRAME_W 1024
#define PORT_GPU_FRAME_H 512

/* The most packets one DrawOTag follows. A chain of distinct packets cannot be
 * longer than the RAM has words (2 MB / 4), so a longer one is a cycle. */
#define PORT_GPU_MAX_PACKETS 0x80000u

#endif
