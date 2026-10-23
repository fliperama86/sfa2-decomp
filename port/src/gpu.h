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
/* Startup flags from shared settings. No GPU/window initialization here.
 * Returns -1 if an enhancement is requested in a build without PsyZ. */
int port_gpu_set_video_options(int pixel_perfect_ui,int arcade_video);
/* Default-off common UI centering, active only in the wider game canvas. */
int port_gpu_set_ui_layout(int enabled);
/* The picture's filter ([display] filter, enum port_filter of settings.h): -1 for anything but nearest without the patched
 * SDL_GPU backend. Takes effect at the next present. */
int port_gpu_set_filter(int filter);
/* The screen ([display] screen, enum port_screen): w, h the window's inner size for PORT_SCREEN_WINDOW, the mode's size for
 * PORT_SCREEN_FULLSCREEN (0, 0 the desktop's), ignored for PORT_SCREEN_BORDERLESS. Kept before the window exists and applied
 * when it opens; applied at once after. -1 without the patched SDL_GPU backend for anything but the default window, or when
 * the backend refuses (the screen then stays as it was). */
int port_gpu_set_screen(int screen, int w, int h);
/* The screen in effect (a fullscreen that the system refused leaves PORT_SCREEN_WINDOW). */
int port_gpu_screen(void);
/* The desktop's size of the window's display: 0, or -1 when there is no window yet. */
int port_gpu_desktop_size(int *w, int *h);
/* The fullscreen sizes of the window's display, as w, h pairs, largest first, no repeats; the count (0 when there is no window). */
int port_gpu_fullscreen_modes(int *wh, int max);
/* Host keys (setting settings_menu). With port_gpu_set_host_keys(1) PsyZ's Escape no longer ends the program and its F4 no
 * longer toggles fullscreen: they and F1 and a gamepad's guide button are kept for port_gpu_take_keys, which returns the
 * PORT_GPU_KEY_* pressed since its last call. With 0 (the default) PsyZ keeps its own keys and take_keys returns 0. */
#define PORT_GPU_KEY_MENU       1u   /* F1 */
#define PORT_GPU_KEY_ESCAPE     2u
#define PORT_GPU_KEY_FULLSCREEN 4u   /* F4 */
#define PORT_GPU_KEY_GUIDE      8u   /* a gamepad's guide (home) button */
void port_gpu_set_host_keys(int on);
unsigned port_gpu_take_keys(void);

/* For the settings menu (menu_run.c), on the game's thread while the game is paused.
 *  - menu_allowed: 1 when the game has drawn, its display is on and it has loaded its font page (a LoadImage reached the page at
 *    (0x3c0, 0x100)).
 *  - menu_snapshot: the video memory (1024 x 512 halfwords) after the drawing so far, into the caller's buffer; 0 or -1.
 *  - menu_begin: shows a display of the menu's own, PORT_MENU_W x PORT_MENU_H, at the game's display's place (moved in where it
 *    would pass the edge of the video memory); its place in *x, *y, and the game's display rectangle in *gx .. *gh (0 x 0 for a
 *    24-bit display, whose picture the menu does not read).
 *  - menu_show: writes w x h halfwords at (x, y) of the video memory, from a host buffer.
 *  - menu_end: the game's display as the game last set it. */
int port_gpu_menu_allowed(void);
int port_gpu_menu_snapshot(uint16_t *vram);
void port_gpu_menu_begin(int *x, int *y, int *gx, int *gy, int *gw, int *gh);
void port_gpu_menu_show(const uint16_t *pixels, int x, int y, int w, int h);
void port_gpu_menu_end(void);

/* Native intro host-buffer display. begin resets callbacks/GPU, frame takes
 * 384x224 RGB555 pixels, end disables display/canvas before normal game setup. */
int port_gpu_intro_begin(void);
void port_gpu_intro_frame(const uint16_t *pixels);
void port_gpu_intro_end(void);

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
