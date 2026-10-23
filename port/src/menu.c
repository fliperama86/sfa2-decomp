/* The PC settings menu: state machine, value lists, texts and picture (port/settings-menu.md, "The menu" and "Drawing").
 * Pure: no PsyZ, no game memory, no files, no globals that change. The host side is menu_run.c. */
#include "menu.h"
#include <stdio.h>
#include <string.h>

/* input.c's pad bits */
#define PAD_TRIANGLE (1u << 4)
#define PAD_CIRCLE   (1u << 5)
#define PAD_CROSS    (1u << 6)
#define PAD_START    (1u << 11)
#define PAD_UP       (1u << 12)
#define PAD_RIGHT    (1u << 13)
#define PAD_DOWN     (1u << 14)
#define PAD_LEFT     (1u << 15)
#define PAD_DIRS     (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT)

#define REPEAT_FIRST 18   /* steps a direction is held before it repeats */
#define REPEAT_NEXT  5    /* steps between repeats after that */

#define SIZES_CAP (PORT_MENU_MODES_MAX + 16)

static const int window_sizes[][2] = {
    {640, 480}, {960, 720}, {1280, 960}, {1440, 1080}, {1600, 1200}, {1920, 1440}, {2560, 1920}, {2880, 2160},
};
#define WINDOW_SIZES ((int)(sizeof window_sizes / sizeof window_sizes[0]))

static const char *const hint = "UP/DOWN SELECT  LEFT/RIGHT CHANGE  F1 BACK";

void port_menu_open(struct port_menu *m, unsigned pad)
{
    m->row = PORT_MENU_DISPLAY;
    m->last = pad;        /* what is held now has no press edge, so it does nothing until released and pressed again */
    m->held_frames = 0;
    m->held_dir = 0;      /* a direction is only followed from its own press edge */
}

int port_menu_row_enabled(int row, const struct port_settings *s)
{
    if (row < 0 || row >= PORT_MENU_ROWS) return 0;
    if (row == PORT_MENU_RESOLUTION) return s->screen != PORT_SCREEN_BORDERLESS;
    return 1;
}

/* ---- the value list of RESOLUTION ---- */

static long long area(int w, int h) { return (long long)w * h; }

static int find_size(const int *wh, int n, int w, int h)
{
    int i;
    for (i = 0; i < n; i++)
        if (wh[2 * i] == w && wh[2 * i + 1] == h) return i;
    return -1;
}

static void insert_size(int *wh, int *n, int at, int w, int h)
{
    memmove(wh + 2 * (at + 1), wh + 2 * at, sizeof(int) * 2 * (size_t)(*n - at));
    wh[2 * at] = w;
    wh[2 * at + 1] = h;
    (*n)++;
}

int port_menu_sizes(const struct port_settings *s, const struct port_menu_host *host, int *wh, int max)
{
    int list[2 * SIZES_CAP], n = 0, i, modes = 0, dw = 0, dh = 0;
    if (host) {
        dw = host->desktop_w;
        dh = host->desktop_h;
        modes = host->modes;
        if (modes < 0) modes = 0;
        if (modes > PORT_MENU_MODES_MAX) modes = PORT_MENU_MODES_MAX;
    }
    if (s->screen == PORT_SCREEN_WINDOW) {
        for (i = 0; i < WINDOW_SIZES; i++) {
            if (dw > 0 && dh > 0 && (window_sizes[i][0] > dw || window_sizes[i][1] > dh)) continue;
            list[2 * n] = window_sizes[i][0];
            list[2 * n + 1] = window_sizes[i][1];
            n++;
        }
        if (!n) { list[0] = window_sizes[0][0]; list[1] = window_sizes[0][1]; n = 1; }
        if (find_size(list, n, s->window_w, s->window_h) < 0) {
            long long a = area(s->window_w, s->window_h);
            for (i = 0; i < n && area(list[2 * i], list[2 * i + 1]) <= a; i++) {}
            insert_size(list, &n, i, s->window_w, s->window_h);
        }
    } else {
        list[0] = 0; list[1] = 0; n = 1;                       /* DESKTOP */
        if (s->screen == PORT_SCREEN_FULLSCREEN) {
            for (i = 0; i < modes; i++) {
                list[2 * n] = host->mode_wh[2 * i];
                list[2 * n + 1] = host->mode_wh[2 * i + 1];
                n++;
            }
            if ((s->fullscreen_w || s->fullscreen_h) && find_size(list, n, s->fullscreen_w, s->fullscreen_h) < 0)
                insert_size(list, &n, 1, s->fullscreen_w, s->fullscreen_h);
        }
    }
    if (n > max) n = max < 0 ? 0 : max;
    memcpy(wh, list, sizeof(int) * 2 * (size_t)n);
    return n;
}

/* ---- the step ---- */

static int cycle(int value, int count, int delta) { return ((value + delta) % count + count) % count; }

/* Move the value of a row by delta (+1 or -1). Returns 1 when something changed. */
static int change_value(int row, int delta, const struct port_menu_host *host, struct port_settings *s)
{
    if (!port_menu_row_enabled(row, s)) return 0;
    if (row == PORT_MENU_DISPLAY) {
        s->screen = cycle(s->screen, 3, delta);   /* the stored sizes stay as they are */
        return 1;
    }
    if (row == PORT_MENU_FILTER) {
        s->filter = cycle(s->filter, 3, delta);
        return 1;
    }
    if (row == PORT_MENU_RESOLUTION) {
        int wh[2 * SIZES_CAP], n, at, next, *w, *h;
        int fullscreen = s->screen == PORT_SCREEN_FULLSCREEN;
        w = fullscreen ? &s->fullscreen_w : &s->window_w;
        h = fullscreen ? &s->fullscreen_h : &s->window_h;
        n = port_menu_sizes(s, host, wh, SIZES_CAP);
        if (n < 1) return 0;
        at = find_size(wh, n, *w, *h);
        next = at < 0 ? 0 : cycle(at, n, delta);
        if (next == at) return 0;
        *w = wh[2 * next];
        *h = wh[2 * next + 1];
        return 1;
    }
    return 0;
}

static void move_row(struct port_menu *m, int delta, const struct port_settings *s)
{
    int i, row = m->row;
    for (i = 0; i < PORT_MENU_ROWS; i++) {
        row = cycle(row, PORT_MENU_ROWS, delta);
        if (port_menu_row_enabled(row, s)) { m->row = row; return; }
    }
}

int port_menu_step(struct port_menu *m, unsigned pad, const struct port_menu_host *host, struct port_settings *s)
{
    unsigned edge = pad & ~m->last, dir = 0;
    m->last = pad;
    if (edge & PAD_DIRS) {                       /* a new direction: act now, and time its repeat from here */
        dir = edge & PAD_UP ? PAD_UP : edge & PAD_DOWN ? PAD_DOWN : edge & PAD_LEFT ? PAD_LEFT : PAD_RIGHT;
        m->held_dir = dir;
        m->held_frames = 0;
    } else if (m->held_dir) {
        if (pad & m->held_dir) {
            m->held_frames++;
            if (m->held_frames == REPEAT_FIRST ||
                (m->held_frames > REPEAT_FIRST && (m->held_frames - REPEAT_FIRST) % REPEAT_NEXT == 0))
                dir = m->held_dir;
        } else {
            m->held_dir = 0;
            m->held_frames = 0;
        }
    }
    if (edge & (PAD_CIRCLE | PAD_TRIANGLE)) return PORT_MENU_CLOSE;
    if (edge & (PAD_CROSS | PAD_START)) {
        if (m->row == PORT_MENU_RETURN) return PORT_MENU_CLOSE;
        if (m->row == PORT_MENU_QUIT) return PORT_MENU_QUIT_GAME;
        return change_value(m->row, 1, host, s) ? PORT_MENU_CHANGED : PORT_MENU_NONE;
    }
    if (dir == PAD_UP) move_row(m, -1, s);
    else if (dir == PAD_DOWN) move_row(m, 1, s);
    else if (dir == PAD_LEFT) return change_value(m->row, -1, host, s) ? PORT_MENU_CHANGED : PORT_MENU_NONE;
    else if (dir == PAD_RIGHT) return change_value(m->row, 1, host, s) ? PORT_MENU_CHANGED : PORT_MENU_NONE;
    return PORT_MENU_NONE;
}

/* ---- texts ---- */

void port_menu_row_text(int row, const struct port_settings *s, char *label, size_t lsize, char *value, size_t vsize)
{
    static const char *const labels[PORT_MENU_ROWS] = { "DISPLAY", "RESOLUTION", "FILTER", "RETURN TO GAME", "QUIT GAME" };
    static const char *const screens[] = { "WINDOW", "FULLSCREEN", "BORDERLESS" };
    static const char *const filters[] = { "NEAREST", "BILINEAR", "SHARP BILINEAR" };
    char text[48] = "";
    if (row == PORT_MENU_DISPLAY)
        snprintf(text, sizeof text, "%s", s->screen >= 0 && s->screen <= 2 ? screens[s->screen] : "UNKNOWN");
    else if (row == PORT_MENU_FILTER)
        snprintf(text, sizeof text, "%s", s->filter >= 0 && s->filter <= 2 ? filters[s->filter] : "UNKNOWN");
    else if (row == PORT_MENU_RESOLUTION) {
        if (s->screen == PORT_SCREEN_WINDOW) snprintf(text, sizeof text, "%dX%d", s->window_w, s->window_h);
        else if (s->screen == PORT_SCREEN_FULLSCREEN && (s->fullscreen_w || s->fullscreen_h))
            snprintf(text, sizeof text, "%dX%d", s->fullscreen_w, s->fullscreen_h);
        else snprintf(text, sizeof text, "DESKTOP");
    }
    if (label && lsize) snprintf(label, lsize, "%s", row >= 0 && row < PORT_MENU_ROWS ? labels[row] : "");
    if (value && vsize) snprintf(value, vsize, "%s", text);
}

/* ---- the picture ---- */

struct canvas { uint16_t *out; const uint16_t *vram; int w, h; };

/* One glyph of `size` (16 or 8) pixels at (x, y) of the display, clipped to it. */
static void glyph(const struct canvas *c, int x, int y, int size, unsigned code, int pal)
{
    unsigned u0, v0;
    int gx, gy;
    if (code <= 0x20 || code >= 0x80) return;                 /* a space, and what the font has no code for, draws nothing */
    if (size == 16) {
        unsigned v = code - 0x20;
        u0 = ((v & 7) << 4) | 0x80;
        v0 = (v & 0x78) << 1;
    } else {
        u0 = (code & 15) << 3;
        v0 = (code & 0xf0) >> 1;
    }
    for (gy = 0; gy < size; gy++) {
        int py = y + gy;
        if (py < 0 || py >= c->h) continue;
        for (gx = 0; gx < size; gx++) {
            int px = x + gx;
            unsigned u = u0 + (unsigned)gx, half, index, colour;
            if (px < 0 || px >= c->w) continue;
            half = c->vram[(PORT_MENU_FONT_Y + v0 + (unsigned)gy) * PORT_MENU_VRAM_W + PORT_MENU_FONT_X + (u >> 2)];
            index = (half >> ((u & 3) * 4)) & 15;
            colour = c->vram[(unsigned)(PORT_MENU_CLUT_Y + pal) * PORT_MENU_VRAM_W + PORT_MENU_CLUT_X + index];
            if (colour == 0) continue;                         /* entry 0x0000 is transparent */
            c->out[py * c->w + px] = (uint16_t)(colour & 0x7fff);
        }
    }
}

static void text(const struct canvas *c, int x, int y, int size, const char *s, int pal)
{
    for (; *s; s++, x += size) glyph(c, x, y, size, (unsigned char)*s, pal);
}

static int text_width(const char *s, int size) { return (int)strlen(s) * size; }

void port_menu_draw(const struct port_menu *m, const struct port_settings *s, const uint16_t *vram,
                    int x, int y, int w, int h, uint16_t *out)
{
    struct canvas c;
    int i, py, px;
    static const char title[] = "PC SETTINGS";
    c.out = out; c.vram = vram; c.w = PORT_MENU_W; c.h = PORT_MENU_H;
    /* the backdrop: the game's display stretched to the menu's size (nearest), dimmed; black where there is none */
    for (py = 0; py < PORT_MENU_H; py++)
        for (px = 0; px < PORT_MENU_W; px++) {
            unsigned pixel = 0;
            if (w > 0 && h > 0) {
                long long vx = x + (long long)px * w / PORT_MENU_W, vy = y + (long long)py * h / PORT_MENU_H;
                if (vx >= 0 && vx < PORT_MENU_VRAM_W && vy >= 0 && vy < PORT_MENU_VRAM_H)
                    pixel = vram[vy * PORT_MENU_VRAM_W + vx];
            }
            out[py * PORT_MENU_W + px] = (uint16_t)((pixel >> 3) & 0x0c63);   /* each 5-bit channel >> 3, bit 15 cleared */
        }
    text(&c, (PORT_MENU_W - text_width(title, 16)) / 2, 24, 16, title, PORT_MENU_PAL_TITLE);
    for (i = 0; i < PORT_MENU_ROWS; i++) {
        char label[32], value[48];
        int pal, vpal, ry = i < PORT_MENU_RETURN ? 56 + 24 * i : 152 + 24 * (i - PORT_MENU_RETURN);
        port_menu_row_text(i, s, label, sizeof label, value, sizeof value);
        pal = !port_menu_row_enabled(i, s) ? PORT_MENU_PAL_DISABLED
              : i == m->row ? PORT_MENU_PAL_SELECTED : PORT_MENU_PAL_NORMAL;
        vpal = pal == PORT_MENU_PAL_NORMAL ? PORT_MENU_PAL_VALUE : pal;   /* only an unselected, available value is told from its label */
        if (i < PORT_MENU_RETURN) {
            text(&c, 16, ry, 16, label, pal);
            text(&c, PORT_MENU_W - 32 - text_width(value, 16), ry, 16, value, vpal);
        } else {
            text(&c, (PORT_MENU_W - text_width(label, 16)) / 2, ry, 16, label, pal);
        }
    }
    text(&c, (PORT_MENU_W - text_width(hint, 8)) / 2, 216, 8, hint, PORT_MENU_PAL_NORMAL);
}
