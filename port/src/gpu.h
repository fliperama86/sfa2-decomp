/* The graphics layer's interface (package L3). Host routines for the graphics
 * library functions the game calls (ResetGraph, DrawOTag, PutDispEnv, the
 * Set* primitive setters, ...), drawn by PsyZ (see gpu.c).
 *
 * The seams with the driver (L1):
 *  - L1's table code takes port_gpu_library[] (library functions without C,
 *    keyed by the name the build's tables give them) and port_gpu_overrides[]
 *    (game functions WITH C that the port replaces: the C of
 *    func_80158374, the game's PutDrawEnv, calls the library's DMA jump
 *    table through memory that no host routine fills), and writes the jumps.
 *  - L1's port_tick (kernel.c) presents the frame through port_gpu_present,
 *    once per vblank, after the vblank handlers and the disc layer.
 *  - this layer calls port_tick() in DrawSync, the call in which the game
 *    waits for the drawing to end; it calls port_callbacks_reset() for
 *    ResetGraph(0), as PSY-Q's ResetGraph does ResetCallback; and it ends the
 *    program with port_halt(PORT_EXIT_GRAPHICS, ...) for a list it cannot
 *    follow.
 * Without PORT_HAVE_PSYZ (a build that did not link PsyZ) the tables are empty
 * and port_gpu_present does nothing, so every library function of this
 * domain keeps its stop call. hostbuild.py defines PORT_HAVE_PSYZ for gpu.c
 * when it is given --psyz. */
#ifndef PORT_GPU_H
#define PORT_GPU_H

#include "port.h"

extern const struct port_library  port_gpu_library[];    /* ends with a null name */
extern const struct port_override port_gpu_overrides[];  /* ends with a null name */

/* Show the frame that the drawing so far has made and pump the window's
 * events. Never waits, calls no callback of PsyZ's own kernel. A closed window
 * (or Escape) ends the program with status 0 after the line
 * `stop: window closed`. Does nothing until the game has used a graphics
 * routine. Declared in port.h too; the same prototype. */
void port_gpu_present(void);

/* The most packets one DrawOTag follows. A chain of distinct packets cannot be
 * longer than the RAM has words (2 MB / 4), so a longer one is a cycle. */
#define PORT_GPU_MAX_PACKETS 0x80000u

#endif
