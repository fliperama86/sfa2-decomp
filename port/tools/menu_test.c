/* Isolated native fixtures of the settings menu (port/src/menu.c): a generated video memory, no game, no graphics.
 * The expected pictures are painted here from the glyph patterns that the font page was generated from, not by reading
 * the page back, so they do not share the menu's texel arithmetic. */
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures;
#define CHECK(cond) do { if (!(cond)) { failures++; printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } } while (0)

#define PAD_TRIANGLE (1u << 4)
#define PAD_CIRCLE   (1u << 5)
#define PAD_CROSS    (1u << 6)
#define PAD_START    (1u << 11)
#define PAD_UP       (1u << 12)
#define PAD_RIGHT    (1u << 13)
#define PAD_DOWN     (1u << 14)
#define PAD_LEFT     (1u << 15)

/* ---- the generated video memory ---- */

static uint16_t vram[PORT_MENU_VRAM_W * PORT_MENU_VRAM_H];

/* texel index of glyph `code` at (gx, gy); the patterns differ between the two fonts and hit index 0 now and then */
static int pattern(int size, int code, int gx, int gy)
{
    return size == 16 ? (code * 7 + gx * 3 + gy * 5) & 15 : (code * 5 + gx * 5 + gy * 3 + 1) & 15;
}

/* palette p entry i: entry 0 transparent, the others distinct per palette, bit 15 set on odd entries */
static uint16_t colour(int p, int i)
{
    return i ? (uint16_t)(((i & 1) ? 0x8000 : 0) | ((p & 31) << 10) | (i << 5) | ((p * 3 + i) & 31)) : 0;
}

/* the backdrop of the picture at video memory (x, y) */
static uint16_t backdrop(int x, int y)
{
    if (y == 0 && x < 3) return x == 0 ? 0xffff : x == 1 ? 0x8421 : 0x4210;   /* known pixels at the display's corner */
    return (uint16_t)(x * 0x0123 + y * 0x0457 + ((x ^ y) << 11));
}

static void put_texel(int u, int v, int index)
{
    uint16_t *half = &vram[(PORT_MENU_FONT_Y + v) * PORT_MENU_VRAM_W + PORT_MENU_FONT_X + u / 4];
    *half = (uint16_t)(*half | index << ((u & 3) * 4));
}

static void build_vram(void)
{
    int p, i, x, y, code, gx, gy;
    memset(vram, 0, sizeof vram);
    for (y = 0; y < 400; y++) for (x = 0; x < 900; x++) vram[y * PORT_MENU_VRAM_W + x] = backdrop(x, y);
    for (p = 0; p < 32; p++) for (i = 0; i < 16; i++) vram[(PORT_MENU_CLUT_Y + p) * PORT_MENU_VRAM_W + PORT_MENU_CLUT_X + i] = colour(p, i);
    for (code = 0x21; code < 0x80; code++) {
        /* 16 x 16: eight to a row from u 128, rows of 16 from the code 0x20; 8 x 8: sixteen to a row, rows of 8 per 16 codes */
        int u16 = 0x80 + ((code - 0x20) % 8) * 16, v16 = ((code - 0x20) / 8) * 16;
        int u8 = (code % 16) * 8, v8 = (code / 16) * 8;
        for (gy = 0; gy < 16; gy++) for (gx = 0; gx < 16; gx++) put_texel(u16 + gx, v16 + gy, pattern(16, code, gx, gy));
        for (gy = 0; gy < 8; gy++) for (gx = 0; gx < 8; gx++) put_texel(u8 + gx, v8 + gy, pattern(8, code, gx, gy));
    }
}

/* ---- the reference picture ---- */

static uint16_t dimmed(uint16_t c)
{
    int r = (c & 31) >> 3, g = ((c >> 5) & 31) >> 3, b = ((c >> 10) & 31) >> 3;
    return (uint16_t)(r | g << 5 | b << 10);
}

#define W PORT_MENU_W
#define H PORT_MENU_H
/* the source rectangle of the game's display in the video memory copy, and the picture being painted (W x H) */
struct ref { uint16_t *img; int sx, sy, sw, sh; };

/* a video memory pixel; outside the video memory 0 */
static uint16_t src_pixel(int x, int y)
{
    return x >= 0 && x < PORT_MENU_VRAM_W && y >= 0 && y < PORT_MENU_VRAM_H ? vram[y * PORT_MENU_VRAM_W + x] : 0;
}

/* the backdrop pixel of picture pixel (px, py): the dimmed source pixel, nearest; black without a source */
static uint16_t backdrop_at(int sx, int sy, int sw, int sh, int px, int py)
{
    return sw > 0 && sh > 0 ? dimmed(src_pixel(sx + px * sw / W, sy + py * sh / H)) : 0;
}

static void ref_start(struct ref *r, uint16_t *img, int sx, int sy, int sw, int sh)
{
    int x, y;
    r->img = img; r->sx = sx; r->sy = sy; r->sw = sw; r->sh = sh;
    for (y = 0; y < H; y++) for (x = 0; x < W; x++) img[y * W + x] = backdrop_at(sx, sy, sw, sh, x, y);
}

static void ref_glyph(struct ref *r, int size, int code, int x, int y, int pal)
{
    int gx, gy;
    if (code == ' ') return;
    for (gy = 0; gy < size; gy++) for (gx = 0; gx < size; gx++) {
        uint16_t c = colour(pal, pattern(size, code, gx, gy));
        int px = x + gx, py = y + gy;
        if (c && px >= 0 && px < W && py >= 0 && py < H) r->img[py * W + px] = c & 0x7fff;
    }
}

static void ref_text(struct ref *r, int size, const char *s, int x, int y, int pal)
{
    for (; *s; s++, x += size) ref_glyph(r, size, (unsigned char)*s, x, y, pal);
}

static const char hint[] = "UP/DOWN SELECT  LEFT/RIGHT CHANGE  F1 BACK";

/* the whole menu as the spec lays it out: value strings and the palette of each of the five rows are given */
struct layout { const char *value[3]; int pal[5]; int vpal[3]; int title_dx, title_pal; };

static void ref_menu(struct ref *r, const struct layout *l)
{
    static const char *label[5] = { "DISPLAY", "RESOLUTION", "FILTER", "RETURN TO GAME", "QUIT GAME" };
    int i, w = W;
    ref_text(r, 16, "PC SETTINGS", (w - 11 * 16) / 2 + l->title_dx, 24, l->title_pal);
    for (i = 0; i < 3; i++) {
        ref_text(r, 16, label[i], 16, 56 + 24 * i, l->pal[i]);
        ref_text(r, 16, l->value[i], w - 32 - 16 * (int)strlen(l->value[i]), 56 + 24 * i, l->vpal[i]);
    }
    ref_text(r, 16, label[3], (w - 16 * 14) / 2, 152, l->pal[3]);
    ref_text(r, 16, label[4], (w - 16 * 9) / 2, 176, l->pal[4]);
    ref_text(r, 8, hint, (w - 8 * (int)strlen(hint)) / 2, 216, 0x1a);
}

static int diff(const uint16_t *a, const uint16_t *b, int n)
{
    int i, d = 0;
    for (i = 0; i < n; i++) d += a[i] != b[i];
    return d;
}

static uint16_t outbuf[W * H + 200], refbuf[W * H];

/* draw into outbuf between guard words (nothing outside W * H may be written) and compare with the reference; returns the differing pixels */
static int draw_and_diff(const struct port_menu *m, const struct port_settings *s, int sx, int sy, int sw, int sh, const struct layout *l)
{
    struct ref r;
    uint16_t *out = outbuf + 100;
    int i, d;
    for (i = 0; i < W * H + 200; i++) outbuf[i] = 0xa5a5;
    port_menu_draw(m, s, vram, sx, sy, sw, sh, out);
    for (i = 0; i < 100; i++) CHECK(outbuf[i] == 0xa5a5 && outbuf[100 + W * H + i] == 0xa5a5);
    ref_start(&r, refbuf, sx, sy, sw, sh);
    ref_menu(&r, l);
    d = diff(out, refbuf, W * H);
    return d;
}

static void defaults(struct port_settings *s)
{
    memset(s, 0, sizeof *s);
    s->screen = PORT_SCREEN_WINDOW; s->window_w = 1280; s->window_h = 960;
    s->filter = PORT_FILTER_SHARP_BILINEAR;
}

/* compare one glyph's pixels in `out` (W wide) with the pattern at (x, y) in `pal`; transparent texels must show the dimmed backdrop
 * of the source rectangle sx, sy, sw, sh */
static int glyph_diff(const uint16_t *out, int sx, int sy, int sw, int sh, int size, int code, int x, int y, int pal)
{
    int gx, gy, d = 0;
    for (gy = 0; gy < size; gy++) for (gx = 0; gx < size; gx++) {
        uint16_t c = colour(pal, pattern(size, code, gx, gy));
        uint16_t want = c ? (uint16_t)(c & 0x7fff) : backdrop_at(sx, sy, sw, sh, x + gx, y + gy);
        d += out[(y + gy) * W + x + gx] != want;
    }
    return d;
}

static void test_drawing(void)
{
    struct port_menu m;
    struct port_settings s;
    struct layout l;
    int i, transparent = 0, w = 384, h = 240;
    uint16_t *out = outbuf + 100;
    build_vram();
    defaults(&s);
    port_menu_open(&m, 0);
    l = (struct layout){ { "WINDOW", "1280X960", "SHARP BILINEAR" }, { 0x10, 0x1a, 0x1a, 0x1a, 0x1a }, { 0x10, 0x16, 0x16 }, 0, 0x1e };

    /* the whole picture, at the display's corner */
    CHECK(draw_and_diff(&m, &s, 0, 0, w, h, &l) == 0);
    port_menu_draw(&m, &s, vram, 0, 0, w, h, out);
    /* known backdrop pixels: dimmed to an eighth per channel, bit 15 cleared */
    CHECK(out[0] == 0x0c63 && out[1] == 0x0000 && out[2] == 0x0842);
    CHECK(out[0] != 0x1ce7 && out[2] != 0x1084);        /* the old shift by two would give these */
    /* exact glyph pixels at the spec's positions: title P (104, 24) in 0x1e, DISPLAY's D (16, 56) selected in 0x10, RESOLUTION's R in 0x1a,
     * the value S of SHARP BILINEAR (128, 104), RETURN's R centred (80, 152), the 8 x 8 hint U (24, 216) */
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'P', 104, 24, 0x1e) == 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'D', 16, 56, 0x10) == 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'R', 16, 80, 0x1a) == 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'S', 128, 104, 0x16) == 0);       /* a value of an unselected row: palette 0x16 */
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, '1', 224, 80, 0x16) == 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'W', 256, 56, 0x10) == 0);        /* the selected row's value: 0x10 like its label */
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'F', 16, 104, 0x1a) == 0);        /* its label stays 0x1a */
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'R', 80, 152, 0x1a) == 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'Q', 120, 176, 0x1a) == 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 8, 'U', 24, 216, 0x1a) == 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 8, '/', 24 + 16, 216, 0x1a) == 0);
    for (i = 0; i < 256; i++) transparent += pattern(16, 'P', i % 16, i / 16) == 0;
    CHECK(transparent > 8);   /* the checked glyph really has transparent texels, which glyph_diff compared with the backdrop */
    /* negative controls: the same checks against a wrong palette or a shifted position must fail */
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'P', 104, 24, 0x1a) != 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'P', 105, 24, 0x1e) != 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'D', 16, 56, 0x1a) != 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'S', 128, 104, 0x1a) != 0);       /* the value in its label's palette */
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'W', 256, 56, 0x16) != 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 8, 'U', 24, 217, 0x1a) != 0);
    {
        struct layout bad = l;
        bad.title_pal = 0x1a;
        CHECK(draw_and_diff(&m, &s, 0, 0, w, h, &bad) != 0);
        bad = l; bad.title_dx = 1;
        CHECK(draw_and_diff(&m, &s, 0, 0, w, h, &bad) != 0);
        bad = l; bad.pal[0] = 0x1a;
        CHECK(draw_and_diff(&m, &s, 0, 0, w, h, &bad) != 0);
        bad = l; bad.vpal[2] = 0x1a;                 /* an unselected value drawn in the palette of its label */
        CHECK(draw_and_diff(&m, &s, 0, 0, w, h, &bad) != 0);
        bad = l; bad.vpal[0] = 0x16;                 /* the selected value not following its label */
        CHECK(draw_and_diff(&m, &s, 0, 0, w, h, &bad) != 0);
        bad = l; bad.pal[2] = 0x10;
        CHECK(draw_and_diff(&m, &s, 0, 0, w, h, &bad) != 0);
    }

    /* other states: borderless with the filter selected (RESOLUTION unavailable, 0x1b) */
    s.screen = PORT_SCREEN_BORDERLESS; s.filter = PORT_FILTER_NEAREST; m.row = PORT_MENU_FILTER;
    l = (struct layout){ { "BORDERLESS", "DESKTOP", "NEAREST" }, { 0x1a, 0x1b, 0x10, 0x1a, 0x1a }, { 0x16, 0x1b, 0x10 }, 0, 0x1e };
    CHECK(draw_and_diff(&m, &s, 0, 0, w, h, &l) == 0);
    port_menu_draw(&m, &s, vram, 0, 0, w, h, out);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'R', 16, 80, 0x1b) == 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'R', 16, 80, 0x1a) != 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'F', 16, 104, 0x10) == 0);
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'D', 240, 80, 0x1b) == 0);        /* unavailable: the value in 0x1b as well */
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'N', 240, 104, 0x10) == 0);      /* selected FILTER's value */
    CHECK(glyph_diff(out, 0, 0, 384, 240, 16, 'D', 240, 80, 0x16) != 0);
    /* fullscreen at a listed size, QUIT GAME selected */
    s.screen = PORT_SCREEN_FULLSCREEN; s.fullscreen_w = 1920; s.fullscreen_h = 1080; s.filter = PORT_FILTER_BILINEAR; m.row = PORT_MENU_QUIT;
    l = (struct layout){ { "FULLSCREEN", "1920X1080", "BILINEAR" }, { 0x1a, 0x1a, 0x1a, 0x1a, 0x10 }, { 0x16, 0x16, 0x16 }, 0, 0x1e };
    CHECK(draw_and_diff(&m, &s, 0, 0, w, h, &l) == 0);

    /* the source is the game's display in the video memory: identity for 384 x 240, here at an offset */
    defaults(&s); m.row = PORT_MENU_RESOLUTION;
    l = (struct layout){ { "WINDOW", "1280X960", "SHARP BILINEAR" }, { 0x1a, 0x10, 0x1a, 0x1a, 0x1a }, { 0x16, 0x10, 0x16 }, 0, 0x1e };
    CHECK(draw_and_diff(&m, &s, 37, 29, 384, 240, &l) == 0);
    CHECK(draw_and_diff(&m, &s, 11, 3, 384, 240, &l) == 0);
    port_menu_draw(&m, &s, vram, 37, 29, 384, 240, out);
    CHECK(out[0] == dimmed(backdrop(37, 29)) && out[383] == dimmed(backdrop(37 + 383, 29)) && out[239 * 384] == dimmed(backdrop(37, 29 + 239)));
    CHECK(glyph_diff(out, 37, 29, 384, 240, 16, 'P', 104, 24, 0x1e) == 0);
    CHECK(glyph_diff(out, 37, 29, 384, 240, 16, 'R', 16, 80, 0x10) == 0);
    CHECK(glyph_diff(out, 37, 29, 384, 240, 8, 'U', 24, 216, 0x1a) == 0);
    /* a 320 x 240 source (the title screen) is stretched in x: out column c shows source column c * 320 / 384 */
    CHECK(draw_and_diff(&m, &s, 0, 0, 320, 240, &l) == 0);
    CHECK(draw_and_diff(&m, &s, 40, 20, 320, 240, &l) == 0);
    port_menu_draw(&m, &s, vram, 0, 0, 320, 240, out);
    CHECK(out[0] == 0x0c63);                              /* source (0, 0) is 0xffff, dimmed */
    CHECK(out[5] == dimmed(backdrop(4, 0)) && out[6] == dimmed(backdrop(5, 0)) && out[383] == dimmed(backdrop(319, 0)));
    CHECK(out[1] == 0x0c63 && out[2] == 0x0000 && out[3] == 0x0842);   /* source columns 0, 1, 2 (1 * 320 / 384 = 0, 2 * 320 / 384 = 1) */
    CHECK(out[4] == dimmed(backdrop(3, 0)));
    /* a 384 x 480 source halves the rows: out row r shows source row 2 * r (column 383 has no text in the rows above the title) */
    CHECK(draw_and_diff(&m, &s, 0, 0, 384, 480, &l) == 0);
    CHECK(draw_and_diff(&m, &s, 9, 30, 384, 480, &l) == 0);
    port_menu_draw(&m, &s, vram, 9, 30, 384, 480, out);
    CHECK(out[1 * 384 + 383] == dimmed(backdrop(9 + 383, 30 + 2)) && out[5 * 384 + 383] == dimmed(backdrop(9 + 383, 30 + 10)) &&
          out[10 * 384 + 383] == dimmed(backdrop(9 + 383, 30 + 20)));
    /* a source partly or wholly outside the video memory reads 0 there */
    CHECK(draw_and_diff(&m, &s, 700, 300, 384, 240, &l) == 0);
    CHECK(draw_and_diff(&m, &s, -10, -5, 384, 240, &l) == 0);
    port_menu_draw(&m, &s, vram, 700, 300, 384, 240, out);
    CHECK(out[0] == dimmed(backdrop(700, 300)) && out[199] == dimmed(backdrop(899, 300)) && out[324] == 0 && out[383] == 0);
    CHECK(dimmed(backdrop(0, 301)) != 0);               /* what a wrapped read of source column 1024 would show */
    /* no display (w or h not positive): a black backdrop, the text still drawn */
    CHECK(draw_and_diff(&m, &s, 0, 0, 0, 0, &l) == 0);
    CHECK(draw_and_diff(&m, &s, 0, 0, 0, 240, &l) == 0);
    CHECK(draw_and_diff(&m, &s, 0, 0, 384, 0, &l) == 0);
    CHECK(draw_and_diff(&m, &s, 0, 0, -384, 240, &l) == 0);
    port_menu_draw(&m, &s, vram, 0, 0, 0, 0, out);
    CHECK(out[0] == 0x0000 && out[383] == 0x0000 && out[239 * 384 + 383] == 0x0000);
    CHECK(glyph_diff(out, 0, 0, 0, 0, 16, 'P', 104, 24, 0x1e) == 0);
    CHECK(glyph_diff(out, 0, 0, 0, 0, 16, 'P', 104, 24, 0x1a) != 0);
    /* negative control: a picture drawn from one source differs from the reference of a source one pixel along */
    port_menu_draw(&m, &s, vram, 0, 0, 384, 240, out);
    CHECK(out[10 * 384 + 10] == dimmed(backdrop(10, 10)) && out[10 * 384 + 10] != dimmed(backdrop(11, 10)));

    /* clipping: a value so long that it starts left of the picture; nothing outside W * H is written
     * (draw_and_diff checks the guard words) and what is inside equals the clipped reference */
    defaults(&s); m.row = 0;
    s.window_w = -2147483647; s.window_h = -2147483647;
    l = (struct layout){ { "WINDOW", "-2147483647X-2147483647", "SHARP BILINEAR" }, { 0x10, 0x1a, 0x1a, 0x1a, 0x1a }, { 0x10, 0x16, 0x16 }, 0, 0x1e };
    CHECK(draw_and_diff(&m, &s, 0, 0, 384, 240, &l) == 0);
}

/* ---- value lists ---- */

static int has(const int *wh, int n, int w, int h)
{
    int i;
    for (i = 0; i < n; i++) if (wh[2 * i] == w && wh[2 * i + 1] == h) return 1;
    return 0;
}

static int same(const int *wh, int n, const int *want, int count)
{
    return n == count && !memcmp(wh, want, sizeof(int) * 2 * (size_t)count);
}

static void test_sizes(void)
{
    struct port_settings s;
    struct port_menu_host host;
    int wh[2 * (PORT_MENU_MODES_MAX + 16)], n;
    memset(&host, 0, sizeof host);
    defaults(&s);
    {   /* desktop unknown: all eight */
        static const int want[] = { 640,480, 960,720, 1280,960, 1440,1080, 1600,1200, 1920,1440, 2560,1920, 2880,2160 };
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(same(wh, n, want, 8));
    }
    {   /* filtered by the desktop: 1920 x 1080 keeps the sizes that fit in both directions */
        static const int want[] = { 640,480, 960,720, 1280,960, 1440,1080 };
        host.desktop_w = 1920; host.desktop_h = 1080;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(same(wh, n, want, 4));
    }
    {   /* a desktop that is exactly one size keeps it */
        static const int want[] = { 640,480, 960,720, 1280,960 };
        host.desktop_w = 1280; host.desktop_h = 960;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(same(wh, n, want, 3));
    }
    {   /* nothing fits: 640X480 (and the current size, which is not in it, by area) */
        static const int want[] = { 640,480, 1280,960 };
        host.desktop_w = 600; host.desktop_h = 400;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(same(wh, n, want, 2));
        s.window_w = 640; s.window_h = 480;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(n == 1 && wh[0] == 640 && wh[1] == 480);
        host.desktop_w = 5000; host.desktop_h = 400;    /* wide enough but not tall: the height filters too */
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(n == 1);
        s.window_w = 1280; s.window_h = 960;
    }
    host.desktop_w = 0; host.desktop_h = 0;
    {   /* the current size inserted in order of area: in the middle, first, last */
        static const int mid[] = { 640,480, 960,720, 1000,800, 1280,960, 1440,1080, 1600,1200, 1920,1440, 2560,1920, 2880,2160 };
        s.window_w = 1000; s.window_h = 800;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(same(wh, n, mid, 9));
        s.window_w = 320; s.window_h = 240;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(n == 9 && wh[0] == 320 && wh[1] == 240 && wh[2] == 640);
        s.window_w = 5000; s.window_h = 4000;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(n == 9 && wh[16] == 5000 && wh[17] == 4000 && wh[14] == 2880);
        s.window_w = 1280; s.window_h = 960;
    }
    {   /* the count is limited by max */
        n = port_menu_sizes(&s, &host, wh, 3);
        CHECK(n == 3);
    }
    /* fullscreen: DESKTOP first, then the display's sizes as given */
    host.modes = 3;
    { static const int m[] = { 1920,1080, 1280,720, 640,480 }; memcpy(host.mode_wh, m, sizeof m); }
    s.screen = PORT_SCREEN_FULLSCREEN; s.fullscreen_w = 0; s.fullscreen_h = 0;
    {
        static const int want[] = { 0,0, 1920,1080, 1280,720, 640,480 };
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(same(wh, n, want, 4));
        s.fullscreen_w = 1280; s.fullscreen_h = 720;     /* listed: nothing inserted */
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(same(wh, n, want, 4));
    }
    {   /* not listed: after DESKTOP, before the largest */
        static const int want[] = { 0,0, 800,600, 1920,1080, 1280,720, 640,480 };
        s.fullscreen_w = 800; s.fullscreen_h = 600;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(same(wh, n, want, 5));
    }
    {   /* no listed modes at all */
        host.modes = 0;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(n == 2 && wh[0] == 0 && wh[1] == 0 && wh[2] == 800 && wh[3] == 600);
        s.fullscreen_w = 0; s.fullscreen_h = 0;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(n == 1 && wh[0] == 0 && wh[1] == 0);
    }
    {   /* the window's size is not in the fullscreen list, and the desktop does not filter it */
        host.modes = 1; host.mode_wh[0] = 4000; host.mode_wh[1] = 3000; host.desktop_w = 800; host.desktop_h = 600;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(n == 2 && has(wh, n, 4000, 3000) && !has(wh, n, 1280, 960));
    }
    {   /* borderless: DESKTOP only, even with modes and a stored fullscreen size */
        s.screen = PORT_SCREEN_BORDERLESS; s.fullscreen_w = 800; s.fullscreen_h = 600;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(n == 1 && wh[0] == 0 && wh[1] == 0);
        CHECK(!port_menu_row_enabled(PORT_MENU_RESOLUTION, &s));
        CHECK(port_menu_row_enabled(PORT_MENU_DISPLAY, &s) && port_menu_row_enabled(PORT_MENU_FILTER, &s) &&
              port_menu_row_enabled(PORT_MENU_RETURN, &s) && port_menu_row_enabled(PORT_MENU_QUIT, &s));
        s.screen = PORT_SCREEN_WINDOW;
        CHECK(port_menu_row_enabled(PORT_MENU_RESOLUTION, &s));
        s.screen = PORT_SCREEN_FULLSCREEN;
        CHECK(port_menu_row_enabled(PORT_MENU_RESOLUTION, &s));
        CHECK(!port_menu_row_enabled(-1, &s) && !port_menu_row_enabled(PORT_MENU_ROWS, &s));
    }
    /* many modes: capped at PORT_MENU_MODES_MAX plus the current size */
    {
        int i;
        host.modes = PORT_MENU_MODES_MAX;
        for (i = 0; i < PORT_MENU_MODES_MAX; i++) { host.mode_wh[2 * i] = 4000 - i; host.mode_wh[2 * i + 1] = 3000 - i; }
        s.fullscreen_w = 777; s.fullscreen_h = 555;
        n = port_menu_sizes(&s, &host, wh, PORT_MENU_MODES_MAX + 16);
        CHECK(n == PORT_MENU_MODES_MAX + 2 && wh[2] == 777);
        CHECK(port_menu_sizes(&s, NULL, wh, PORT_MENU_MODES_MAX + 16) == 2);
    }
}

/* ---- texts ---- */

static int font_char(char c) { return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' '; }

static void text_case(int row, const struct port_settings *s, const char *label, const char *value)
{
    char l[64], v[64];
    int i;
    port_menu_row_text(row, s, l, sizeof l, v, sizeof v);
    CHECK(!strcmp(l, label));
    CHECK(!strcmp(v, value));
    for (i = 0; l[i]; i++) CHECK(font_char(l[i]));
    for (i = 0; v[i]; i++) CHECK(font_char(v[i]));
}

static void test_texts(void)
{
    struct port_settings s;
    char l[64], v[4];
    int i;
    defaults(&s);
    text_case(PORT_MENU_DISPLAY, &s, "DISPLAY", "WINDOW");
    text_case(PORT_MENU_RESOLUTION, &s, "RESOLUTION", "1280X960");
    text_case(PORT_MENU_FILTER, &s, "FILTER", "SHARP BILINEAR");
    text_case(PORT_MENU_RETURN, &s, "RETURN TO GAME", "");
    text_case(PORT_MENU_QUIT, &s, "QUIT GAME", "");
    s.window_w = 16384; s.window_h = 320;
    text_case(PORT_MENU_RESOLUTION, &s, "RESOLUTION", "16384X320");
    s.screen = PORT_SCREEN_FULLSCREEN;
    text_case(PORT_MENU_DISPLAY, &s, "DISPLAY", "FULLSCREEN");
    text_case(PORT_MENU_RESOLUTION, &s, "RESOLUTION", "DESKTOP");
    s.fullscreen_w = 1920; s.fullscreen_h = 1080;
    text_case(PORT_MENU_RESOLUTION, &s, "RESOLUTION", "1920X1080");
    s.screen = PORT_SCREEN_BORDERLESS;
    text_case(PORT_MENU_DISPLAY, &s, "DISPLAY", "BORDERLESS");
    text_case(PORT_MENU_RESOLUTION, &s, "RESOLUTION", "DESKTOP");
    s.filter = PORT_FILTER_NEAREST;
    text_case(PORT_MENU_FILTER, &s, "FILTER", "NEAREST");
    s.filter = PORT_FILTER_BILINEAR;
    text_case(PORT_MENU_FILTER, &s, "FILTER", "BILINEAR");
    /* a small buffer is cut and ends in NUL */
    s.window_w = 1280; s.window_h = 960; s.screen = PORT_SCREEN_WINDOW;
    port_menu_row_text(PORT_MENU_RESOLUTION, &s, l, sizeof l, v, sizeof v);
    CHECK(!strcmp(v, "128"));
    port_menu_row_text(PORT_MENU_RESOLUTION, &s, NULL, 0, v, 0);
    for (i = 0; hint[i]; i++) CHECK(hint[i] >= 0x20 && hint[i] < 0x7f);
}

/* ---- input ---- */

static struct port_menu m;
static struct port_settings s;
static struct port_menu_host host;
static int act(unsigned pad) { return port_menu_step(&m, pad, &host, &s); }
static int tap(unsigned pad) { int a = act(pad); CHECK(act(0) == PORT_MENU_NONE); return a; }

static void start(unsigned pad_at_open)
{
    defaults(&s);
    memset(&host, 0, sizeof host);
    port_menu_open(&m, pad_at_open);
}

static void test_input(void)
{
    int i, n, steps[40];
    struct port_settings before;
    start(0);
    CHECK(m.row == PORT_MENU_DISPLAY && m.held_dir == 0);
    /* nothing pressed: nothing happens */
    before = s;
    for (i = 0; i < 30; i++) CHECK(act(0) == PORT_MENU_NONE);
    CHECK(!memcmp(&before, &s, sizeof s) && m.row == 0);

    /* buttons held at open do nothing, even when held long; a release and a new press act */
    start(PAD_CROSS | PAD_RIGHT | PAD_DOWN | PAD_START);
    before = s;
    for (i = 0; i < 60; i++) CHECK(act(PAD_CROSS | PAD_RIGHT | PAD_DOWN | PAD_START) == PORT_MENU_NONE);
    CHECK(!memcmp(&before, &s, sizeof s) && m.row == 0);
    CHECK(act(PAD_RIGHT) == PORT_MENU_NONE);          /* cross, down and start released; right still held */
    for (i = 0; i < 40; i++) CHECK(act(PAD_RIGHT) == PORT_MENU_NONE);
    CHECK(act(0) == PORT_MENU_NONE);
    CHECK(act(PAD_CROSS) == PORT_MENU_CHANGED && s.screen == PORT_SCREEN_FULLSCREEN);
    for (i = 0; i < 30; i++) CHECK(act(PAD_CROSS) == PORT_MENU_NONE);   /* a held button acts once */
    CHECK(s.screen == PORT_SCREEN_FULLSCREEN);
    start(PAD_CIRCLE);                                 /* a held close button does not close */
    CHECK(act(PAD_CIRCLE) == PORT_MENU_NONE);
    CHECK(act(0) == PORT_MENU_NONE);
    CHECK(act(PAD_CIRCLE) == PORT_MENU_CLOSE);

    /* rows: down and up wrap over the five */
    start(0);
    for (i = 1; i <= 4; i++) { CHECK(tap(PAD_DOWN) == PORT_MENU_NONE); CHECK(m.row == i); }
    CHECK(tap(PAD_DOWN) == PORT_MENU_NONE && m.row == 0);
    CHECK(tap(PAD_UP) == PORT_MENU_NONE && m.row == PORT_MENU_QUIT);
    CHECK(tap(PAD_UP) == PORT_MENU_NONE && m.row == PORT_MENU_RETURN);
    /* borderless: the cursor skips RESOLUTION in both directions and at the wrap */
    start(0);
    s.screen = PORT_SCREEN_BORDERLESS;
    CHECK(tap(PAD_DOWN) == PORT_MENU_NONE && m.row == PORT_MENU_FILTER);
    CHECK(tap(PAD_UP) == PORT_MENU_NONE && m.row == PORT_MENU_DISPLAY);
    m.row = PORT_MENU_FILTER;
    CHECK(tap(PAD_UP) == PORT_MENU_NONE && m.row == PORT_MENU_DISPLAY);
    CHECK(tap(PAD_UP) == PORT_MENU_NONE && m.row == PORT_MENU_QUIT);
    CHECK(tap(PAD_DOWN) == PORT_MENU_NONE && m.row == PORT_MENU_DISPLAY);

    /* DISPLAY: left and right wrap, the stored sizes stay; making RESOLUTION unavailable moves nothing */
    start(0);
    s.window_w = 1000; s.window_h = 800; s.fullscreen_w = 800; s.fullscreen_h = 600;
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.screen == PORT_SCREEN_FULLSCREEN);
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.screen == PORT_SCREEN_BORDERLESS && m.row == PORT_MENU_DISPLAY);
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.screen == PORT_SCREEN_WINDOW);
    CHECK(tap(PAD_LEFT) == PORT_MENU_CHANGED && s.screen == PORT_SCREEN_BORDERLESS);
    CHECK(tap(PAD_CROSS) == PORT_MENU_CHANGED && s.screen == PORT_SCREEN_WINDOW);
    CHECK(tap(PAD_START) == PORT_MENU_CHANGED && s.screen == PORT_SCREEN_FULLSCREEN);
    CHECK(tap(PAD_LEFT) == PORT_MENU_CHANGED && s.screen == PORT_SCREEN_WINDOW);
    CHECK(s.window_w == 1000 && s.window_h == 800 && s.fullscreen_w == 800 && s.fullscreen_h == 600 && m.row == 0);

    /* FILTER */
    start(0);
    m.row = PORT_MENU_FILTER;
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.filter == PORT_FILTER_NEAREST);
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.filter == PORT_FILTER_BILINEAR);
    CHECK(tap(PAD_LEFT) == PORT_MENU_CHANGED && s.filter == PORT_FILTER_NEAREST);
    CHECK(tap(PAD_LEFT) == PORT_MENU_CHANGED && s.filter == PORT_FILTER_SHARP_BILINEAR);
    CHECK(tap(PAD_CROSS) == PORT_MENU_CHANGED && s.filter == PORT_FILTER_NEAREST);

    /* RESOLUTION, window, desktop 1920 x 1080: 640, 960, 1280, 1440 */
    start(0);
    m.row = PORT_MENU_RESOLUTION;
    host.desktop_w = 1920; host.desktop_h = 1080;
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.window_w == 1440 && s.window_h == 1080);
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.window_w == 640 && s.window_h == 480);
    CHECK(tap(PAD_LEFT) == PORT_MENU_CHANGED && s.window_w == 1440 && s.window_h == 1080);
    CHECK(tap(PAD_LEFT) == PORT_MENU_CHANGED && s.window_w == 1280 && s.window_h == 960);
    CHECK(s.fullscreen_w == 0 && s.fullscreen_h == 0);
    /* a current size outside the list steps to its neighbours in the list, in area order */
    s.window_w = 1000; s.window_h = 800;
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.window_w == 1280);
    s.window_w = 1000; s.window_h = 800;
    CHECK(tap(PAD_LEFT) == PORT_MENU_CHANGED && s.window_w == 960);
    /* a list of one value changes nothing */
    host.desktop_w = 600; host.desktop_h = 400;
    s.window_w = 640; s.window_h = 480;
    before = s;
    CHECK(tap(PAD_RIGHT) == PORT_MENU_NONE && tap(PAD_CROSS) == PORT_MENU_NONE && tap(PAD_LEFT) == PORT_MENU_NONE);
    CHECK(!memcmp(&before, &s, sizeof s));
    /* fullscreen: DESKTOP is 0, 0 and the others are the modes */
    host.modes = 2; host.mode_wh[0] = 1920; host.mode_wh[1] = 1080; host.mode_wh[2] = 1280; host.mode_wh[3] = 720;
    s.screen = PORT_SCREEN_FULLSCREEN; s.window_w = 1280; s.window_h = 960;
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.fullscreen_w == 1920 && s.fullscreen_h == 1080);
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.fullscreen_w == 1280 && s.fullscreen_h == 720);
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.fullscreen_w == 0 && s.fullscreen_h == 0);
    CHECK(tap(PAD_LEFT) == PORT_MENU_CHANGED && s.fullscreen_w == 1280 && s.fullscreen_h == 720);
    CHECK(s.window_w == 1280 && s.window_h == 960);
    /* the cursor on the current size (not in the list) goes after DESKTOP */
    s.fullscreen_w = 800; s.fullscreen_h = 600;
    CHECK(tap(PAD_LEFT) == PORT_MENU_CHANGED && s.fullscreen_w == 0 && s.fullscreen_h == 0);
    s.fullscreen_w = 800; s.fullscreen_h = 600;
    CHECK(tap(PAD_RIGHT) == PORT_MENU_CHANGED && s.fullscreen_w == 1920);

    /* RETURN and QUIT */
    start(0);
    m.row = PORT_MENU_RETURN;
    before = s;
    CHECK(tap(PAD_LEFT) == PORT_MENU_NONE && tap(PAD_RIGHT) == PORT_MENU_NONE && !memcmp(&before, &s, sizeof s));
    CHECK(tap(PAD_CROSS) == PORT_MENU_CLOSE);
    CHECK(tap(PAD_START) == PORT_MENU_CLOSE);
    m.row = PORT_MENU_QUIT;
    CHECK(tap(PAD_LEFT) == PORT_MENU_NONE && tap(PAD_RIGHT) == PORT_MENU_NONE);
    CHECK(tap(PAD_CROSS) == PORT_MENU_QUIT_GAME);
    CHECK(tap(PAD_START) == PORT_MENU_QUIT_GAME);
    CHECK(tap(PAD_CIRCLE) == PORT_MENU_CLOSE && tap(PAD_TRIANGLE) == PORT_MENU_CLOSE);
    m.row = PORT_MENU_FILTER;
    CHECK(tap(PAD_CIRCLE) == PORT_MENU_CLOSE && tap(PAD_TRIANGLE) == PORT_MENU_CLOSE);
    CHECK(s.filter == PORT_FILTER_SHARP_BILINEAR);

    /* repeat: held right acts on the press, then at the 18th following step, then every 5th */
    start(0);
    m.row = PORT_MENU_FILTER;
    n = 0;
    for (i = 0; i < 40; i++) if (act(PAD_RIGHT) == PORT_MENU_CHANGED) steps[n++] = i;
    CHECK(n == 6 && steps[0] == 0 && steps[1] == 18 && steps[2] == 23 && steps[3] == 28 && steps[4] == 33 && steps[5] == 38);
    CHECK(s.filter == (PORT_FILTER_SHARP_BILINEAR + 6) % 3);
    CHECK(act(0) == PORT_MENU_NONE);
    CHECK(act(PAD_RIGHT) == PORT_MENU_CHANGED);        /* a release and a press start over */
    /* the same for rows */
    start(0);
    n = 0;
    for (i = 0; i < 30; i++) { int row = m.row; act(PAD_DOWN); if (m.row != row) steps[n++] = i; }
    CHECK(n == 4 && steps[0] == 0 && steps[1] == 18 && steps[2] == 23 && steps[3] == 28 && m.row == PORT_MENU_QUIT);
    /* a shorter hold than 18 steps does not repeat, but taps in a row each act */
    start(0);
    m.row = PORT_MENU_FILTER;
    CHECK(act(PAD_LEFT) == PORT_MENU_CHANGED);
    for (i = 0; i < 17; i++) CHECK(act(PAD_LEFT) == PORT_MENU_NONE);
    CHECK(act(0) == PORT_MENU_NONE);
    CHECK(act(PAD_LEFT) == PORT_MENU_CHANGED && act(0) == PORT_MENU_NONE && act(PAD_LEFT) == PORT_MENU_CHANGED);
    /* changing direction while holding: the new direction acts at once and the clock starts over */
    start(0);
    m.row = PORT_MENU_FILTER;
    CHECK(act(PAD_RIGHT) == PORT_MENU_CHANGED);
    for (i = 0; i < 10; i++) CHECK(act(PAD_RIGHT) == PORT_MENU_NONE);
    CHECK(act(PAD_LEFT) == PORT_MENU_CHANGED);
    for (i = 0; i < 17; i++) CHECK(act(PAD_LEFT) == PORT_MENU_NONE);
    CHECK(act(PAD_LEFT) == PORT_MENU_CHANGED);
    CHECK(s.filter == PORT_FILTER_BILINEAR);           /* sharp, right to nearest, left to sharp, the repeat to bilinear */
}

int main(void)
{
    test_texts();
    test_sizes();
    test_input();
    test_drawing();
    if (failures) { printf("%d failures\n", failures); return 1; }
    printf("menu ok\n");
    return 0;
}
