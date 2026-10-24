/* Widescreen step 4 (port/src/wide_objects.c): the overrides of the margin test and the draw phase, with stand-ins for the game's
 * two functions, on memory mapped at the game's addresses. Built with gcc -m32 by test_wide_objects.py; no game, disc or window. */
#include "port.h"
#include "settings.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

struct port_settings port_settings;
extern const struct port_override port_wide_object_overrides[];

#define REGION      0x80100000u
#define REGION_SIZE 0x100000u
#define BOX_MARGIN  0x801aa5e6u
#define OBJ_A       0x80150000u
#define OBJ_B       0x80150400u

static unsigned char *ram(unsigned a) { return (unsigned char *)(uintptr_t)a; }
static void put16(unsigned a, int v) { ram(a)[0] = (unsigned char)v; ram(a)[1] = (unsigned char)(v >> 8); }

/* The game's test as the C has it: drawn flag cleared past 448, else set and (here) counted as pushed for collision. */
static int pushed;
static void stand_in_test(unsigned o)
{
    int x = (int16_t)(ram(o + 0x12)[0] | ram(o + 0x12)[1] << 8) - (int16_t)(ram(BOX_MARGIN)[0] | ram(BOX_MARGIN)[1] << 8);
    if (x + 64 > 512) ram(o)[1] = 0;
    else { ram(o)[1] = 1; pushed++; }
}
static int seen_a, seen_b;   /* the drawn flags the draw phase saw */
static void stand_in_draw(void) { seen_a = ram(OBJ_A)[1]; seen_b = ram(OBJ_B)[1]; }

port_fn port_function_impl(const char *name)
{
    if (!strcmp(name, "clear_or_set_flag_by_pos_x_limit")) return (port_fn)stand_in_test;
    if (!strcmp(name, "select_buffers_and_run_first_setup")) return (port_fn)stand_in_draw;
    return PORT_FN_NULL;
}

static void (*test_o)(unsigned);
static void (*draw_o)(void);

static int frame(int wide, int cam, int xa, int xb)
{
    port_settings.widescreen = wide;
    port_settings.arcade_video = 1;
    put16(BOX_MARGIN, cam);
    put16(OBJ_A + 0x12, xa);
    put16(OBJ_B + 0x12, xb);
    pushed = 0;
    test_o(OBJ_A);
    test_o(OBJ_B);
    draw_o();
    return pushed;
}

int main(void)
{
    unsigned i, cases = 0;
    void *p = mmap((void *)(uintptr_t)REGION, REGION_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    if (p != (void *)(uintptr_t)REGION) { puts("setup: cannot map the game's addresses"); return 2; }
    for (i = 0; port_wide_object_overrides[i].name; i++) {
        if (!strcmp(port_wide_object_overrides[i].name, "clear_or_set_flag_by_pos_x_limit")) test_o = (void (*)(unsigned))port_wide_object_overrides[i].host;
        if (!strcmp(port_wide_object_overrides[i].name, "select_buffers_and_run_first_setup")) draw_o = (void (*)(void))port_wide_object_overrides[i].host;
    }
    assert(test_o && draw_o);
    /* A at 300 right of the camera (inside), B at 500 (past the game's 448, inside the wide view's 576) */
    assert(frame(1, 256, 556, 756) == 1);
    assert(seen_a == 1 && seen_b == 1);                       /* B drawn in the sides */
    assert(ram(OBJ_A)[1] == 1 && ram(OBJ_B)[1] == 0);        /* and hidden again for the game's logic; A as the game left it */
    cases++;
    /* widescreen off: B hidden in the draw phase too */
    assert(frame(0, 256, 556, 756) == 1 && seen_b == 0 && ram(OBJ_B)[1] == 0);
    cases++;
    /* B past the wide view (577 right of the camera): not drawn */
    assert(frame(1, 256, 556, 256 + 577) == 1 && seen_b == 0);
    cases++;
    /* B at the edge of the wide view (576): drawn; never counted for collision */
    assert(frame(1, 256, 556, 256 + 576) == 1 && seen_b == 1 && ram(OBJ_B)[1] == 0);
    cases++;
    /* the note lasts one frame: a frame where B is inside the game's own margin draws it by the game's flag alone */
    assert(frame(1, 256, 556, 256 + 400) == 2 && seen_b == 1 && ram(OBJ_B)[1] == 1);
    cases++;
    /* negative control: the expectation that B is drawn fails with widescreen off */
    frame(0, 256, 556, 756);
    assert(!(seen_b == 1));
    cases++;
    printf("wide objects ok: %u cases\n", cases);
    return 0;
}
