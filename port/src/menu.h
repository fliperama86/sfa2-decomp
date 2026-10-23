/* The PC settings menu (setting settings_menu; port/settings-menu.md). menu.c is pure: no PsyZ, no game memory, no files.
 * It draws with the game's own font and palettes, read from a copy of the video memory, at the places the game's text
 * routine (draw_text_items) reads them, so it keeps working whatever loaded them. menu_run.c is the host side. */
#ifndef PORT_MENU_HEADER_H
#define PORT_MENU_HEADER_H
#include "settings.h"
#include <stdint.h>

#define PORT_MENU_VRAM_W 1024
#define PORT_MENU_VRAM_H 512
/* The game's font page: a 4-bit texture page at (0x3c0, 0x100) (draw_text_items: GetTPage(0, 0, 0x3c0, 0x100)), and its
 * palettes: palette p is the 16 colours at (0x70, 0x1e0 + p) (GetClut(0x70, 0x1e0 + p)). */
#define PORT_MENU_FONT_X 0x3c0
#define PORT_MENU_FONT_Y 0x100
#define PORT_MENU_CLUT_X 0x70
#define PORT_MENU_CLUT_Y 0x1e0
/* The palettes of the game's mode select screen (matched on its picture): title, selected row, row, unavailable row. */
#define PORT_MENU_PAL_TITLE    0x1e
#define PORT_MENU_PAL_SELECTED 0x10
#define PORT_MENU_PAL_NORMAL   0x1a
#define PORT_MENU_PAL_DISABLED 0x1b
/* A setting's value on a row that is not selected, apart from its label (the game's blue). */
#define PORT_MENU_PAL_VALUE    0x16
/* The menu's own display, the size of the game's menus and fights (other screens, the title's 320 x 240 among them, are
 * shown behind it stretched to this size). */
#define PORT_MENU_W 384
#define PORT_MENU_H 240

enum port_menu_row { PORT_MENU_DISPLAY, PORT_MENU_RESOLUTION, PORT_MENU_FILTER, PORT_MENU_RETURN, PORT_MENU_QUIT, PORT_MENU_ROWS };
enum port_menu_action { PORT_MENU_NONE, PORT_MENU_CHANGED, PORT_MENU_CLOSE, PORT_MENU_QUIT_GAME };

#define PORT_MENU_MODES_MAX 64
/* What the host knows about the screen, for the value lists. */
struct port_menu_host {
    int desktop_w, desktop_h;            /* 0, 0 when unknown: the window sizes are then not filtered */
    int modes;                           /* fullscreen sizes in mode_wh, as w, h pairs, largest first, no repeats */
    int mode_wh[2 * PORT_MENU_MODES_MAX];
};

struct port_menu {
    int row;                             /* the selected row, enum port_menu_row */
    unsigned last;                       /* the pad word of the step before */
    unsigned held_frames;                /* how many steps the direction in `held_dir` has been held */
    unsigned held_dir;                   /* the one direction bit being repeated, 0 for none */
};

/* Opens on the first row. `pad` is the pad word at the time: buttons held then do nothing until they are released. */
void port_menu_open(struct port_menu *m, unsigned pad);
/* One frame. `pad` is the pad word (input.c's order: bit 0 L2 ... bit 15 left, 1 = pressed). A changed value is written
 * to *s, and the result is PORT_MENU_CHANGED. */
int port_menu_step(struct port_menu *m, unsigned pad, const struct port_menu_host *host, struct port_settings *s);
/* Is a row selectable now (RESOLUTION is not for borderless)? */
int port_menu_row_enabled(int row, const struct port_settings *s);
/* The value list of RESOLUTION for the screen of *s, as w, h pairs (0, 0 is DESKTOP); returns the count (at most
 * PORT_MENU_MODES_MAX + 16). The current size is always in it. */
int port_menu_sizes(const struct port_settings *s, const struct port_menu_host *host, int *wh, int max);
/* The text of a row's label and value, in the font's codes (ASCII capitals, digits, space); value "" for RETURN and QUIT. */
void port_menu_row_text(int row, const struct port_settings *s, char *label, size_t lsize, char *value, size_t vsize);
/* The picture: `vram` is the copy of the video memory (PORT_MENU_VRAM_W x PORT_MENU_VRAM_H halfwords); x, y, w, h is the
 * game's display rectangle in it (w or h 0: none, as for a 24-bit display). Writes PORT_MENU_W x PORT_MENU_H halfwords to
 * `out`: the game's display stretched to that size (nearest), dimmed, with the menu on it; black where there is none. */
void port_menu_draw(const struct port_menu *m, const struct port_settings *s, const uint16_t *vram,
                    int x, int y, int w, int h, uint16_t *out);
/* menu_run.c: called by kernel.c at the end of every vblank, on the game's thread. Does nothing unless settings_menu is
 * on; otherwise takes the host keys and, when asked and possible, runs the menu until it is closed (the game is paused). */
void port_menu_vblank(void);
#endif
