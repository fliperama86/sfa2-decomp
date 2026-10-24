/* Native fixtures of widescreen's walls (port/src/wide_walls.c, port/widescreen.md "The walls of the wide view"). No game, disc or window.
 *
 * The game's own C of init_sprite_position_wide, clamp_pos_x_to_bounds_and_push_other, push_both_objects_inside_bounds,
 * update_camera_x_from_player_span, move_cam_left, move_cam_right and set_cam_pos_x is compiled as it is and linked with its globals at the
 * game's addresses (the linker's --defsym, from ps1/src/symbols.ld), over a block of memory mapped at those addresses. Three parts:
 *
 *  1. at an extra of 0 (port_wide_walls_extra = 0, wide_ends narrowed) each override and the game's C run on identical copies of the
 *     memory they touch, on seeded pseudo-random fixtures that reach each branch (edges within a pixel of the walls among them); the memory
 *     afterwards is compared byte for byte;
 *  2. at the extra of 64 hand-made fixtures say where the walls stand (the 512 view in place of the 384);
 *  3. with wide_ends centred or anchored, widescreen off, or the arcade video off, each override equals the game's C even at an extra of 64.
 *
 * The follower's callees are the game's own move_cam_left, move_cam_right and set_cam_pos_x, reached through wrappers that count the
 * calls and keep the last arguments. Needs a 32-bit program (the game's C holds 32-bit pointers); test_wide_walls.py builds it.
 * Prints one summary line and exits 0, or the function, the fixture and the first differing address and exits 1. */
#include "port.h"
#include "settings.h"
#include "types.gen.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

/* ---- the game's memory ---- */
#define REGION       0x80180000u
#define REGION_SIZE  0x00040000u
#define PLAYER_LEFT  0x80197fc0u
#define OBJECT_SIZE  0x394u
#define PLAYER_RIGHT (PLAYER_LEFT + OBJECT_SIZE)
#define MAIN_LAYER   0x801aa5d4u
#define LAYER_SIZE   0x90u
#define OTHER_SPRITE 0x801aa800u
#define BOX_MARGIN   0x801aa5e6u
#define REF_OTHER    0x80190460u
#define REF_FIRST    0x8019040cu
#define REF_SECOND   0x80190410u

/* the offsets that wide_walls.c writes as numbers, against the generated structs */
_Static_assert(offsetof(Object, state_0) == 0x004, "state_0");
_Static_assert(offsetof(Object, pos_x) == 0x012, "pos_x");
_Static_assert(offsetof(Object, other) == 0x040, "other");
_Static_assert(offsetof(Object, active_state) == 0x045, "active_state");
_Static_assert(offsetof(Object, health) == 0x05c, "health");
_Static_assert(offsetof(Object, status_flag) == 0x073, "status_flag");
_Static_assert(offsetof(Object, half_width) == 0x154, "half_width");
_Static_assert(offsetof(Object, bound_push_flag) == 0x164, "bound_push_flag");
_Static_assert(offsetof(Object, flag_166_set) == 0x166, "flag_166_set");
_Static_assert(offsetof(Object, field_241) == 0x241, "field_241");
_Static_assert(offsetof(Object, field_261) == 0x261, "field_261");
_Static_assert(sizeof(Object) == 0x394, "Object size");
_Static_assert(offsetof(Sprite, field_22) == 0x022, "Sprite.field_22");
_Static_assert(offsetof(Cam, pos_x) == 0x022, "Cam.pos_x");
_Static_assert(offsetof(Cam, min_x) == 0x044, "Cam.min_x");
_Static_assert(offsetof(Cam, max_x) == 0x046, "Cam.max_x");
_Static_assert(offsetof(Sprite, init_param_a) == 0x044, "Sprite.init_param_a is Cam.min_x");
_Static_assert(offsetof(Sprite, init_param_b) == 0x046, "Sprite.init_param_b is Cam.max_x");
_Static_assert(sizeof(Sprite) <= LAYER_SIZE, "Sprite size");
_Static_assert(sizeof(Cam) <= LAYER_SIZE, "Cam size");

/* the memory a fixture can touch, compared whole after each run: game_state and the refs, the two players, the camera's records */
static const struct { uint32_t addr, len; } windows[] = {
    { 0x80190000u, 0x800u }, { PLAYER_LEFT, 2 * OBJECT_SIZE }, { 0x801aa000u, 0x1000u },
};
#define NWINDOWS (sizeof windows / sizeof *windows)
#define SNAP_SIZE (0x800u + 2 * OBJECT_SIZE + 0x1000u)

struct port_settings port_settings;
struct port_settings port_settings_file;
extern int port_wide_walls_extra;
extern const struct port_override port_wide_wall_overrides[];

/* the game's C (compiled from ps1/src) */
void init_sprite_position_wide(void *sprite, int x, int y);
void clamp_pos_x_to_bounds_and_push_other(void *object);
void push_both_objects_inside_bounds(void *object);
void update_camera_x_from_player_span(void *sprite);
void move_cam_left(void *cam, short step);
void move_cam_right(void *cam, short step);
void set_cam_pos_x(void *cam, int delta);

/* the callees of the follower, counted */
static unsigned n_left, n_right, n_set;
static int last_kind;          /* 'l', 'r' or 's': the callee of the last call */
static int last_arg;           /* its second argument as the follower passed it */
static void wrap_left(unsigned cam, unsigned step) { n_left++; last_kind = 'l'; last_arg = (int16_t)step; move_cam_left((void *)(uintptr_t)cam, (short)step); }
static void wrap_right(unsigned cam, unsigned step) { n_right++; last_kind = 'r'; last_arg = (int16_t)step; move_cam_right((void *)(uintptr_t)cam, (short)step); }
static void wrap_set(unsigned cam, unsigned delta) { n_set++; last_kind = 's'; last_arg = (int)delta; set_cam_pos_x((void *)(uintptr_t)cam, (int)delta); }

/* The stand-in for library.c's lookup: the game's C for every name that wide_walls.c asks for. */
port_fn port_function_impl(const char *name)
{
    if (!strcmp(name, "init_sprite_position_wide")) return (port_fn)init_sprite_position_wide;
    if (!strcmp(name, "clamp_pos_x_to_bounds_and_push_other")) return (port_fn)clamp_pos_x_to_bounds_and_push_other;
    if (!strcmp(name, "push_both_objects_inside_bounds")) return (port_fn)push_both_objects_inside_bounds;
    if (!strcmp(name, "update_camera_x_from_player_span")) return (port_fn)update_camera_x_from_player_span;
    if (!strcmp(name, "move_cam_left")) return (port_fn)wrap_left;
    if (!strcmp(name, "move_cam_right")) return (port_fn)wrap_right;
    if (!strcmp(name, "set_cam_pos_x")) return (port_fn)wrap_set;
    return PORT_FN_NULL;
}

/* ---- memory helpers ---- */
static unsigned char *ram(uint32_t a) { return (unsigned char *)(uintptr_t)a; }
static int rd16(uint32_t a) { return (int16_t)(ram(a)[0] | ram(a)[1] << 8); }
static unsigned rdu16(uint32_t a) { return (unsigned)(ram(a)[0] | ram(a)[1] << 8); }
static uint32_t rd32(uint32_t a) { uint32_t v; memcpy(&v, ram(a), 4); return v; }
static void wr16(uint32_t a, int v) { ram(a)[0] = (unsigned char)v; ram(a)[1] = (unsigned char)(v >> 8); }
static void wr32(uint32_t a, uint32_t v) { memcpy(ram(a), &v, 4); }

static void snapshot(unsigned char *to)
{
    unsigned i;
    for (i = 0; i < NWINDOWS; i++) { memcpy(to, ram(windows[i].addr), windows[i].len); to += windows[i].len; }
}
static void restore(const unsigned char *from)
{
    unsigned i;
    for (i = 0; i < NWINDOWS; i++) { memcpy(ram(windows[i].addr), from, windows[i].len); from += windows[i].len; }
}
/* the first address where the memory differs from `want` (a snapshot), or 0 */
static uint32_t first_difference(const unsigned char *want)
{
    unsigned i, k;
    for (i = 0; i < NWINDOWS; i++) {
        for (k = 0; k < windows[i].len; k++) if (ram(windows[i].addr)[k] != want[k]) return windows[i].addr + k;
        want += windows[i].len;
    }
    return 0;
}
static void clear_windows(void)
{
    unsigned i;
    for (i = 0; i < NWINDOWS; i++) memset(ram(windows[i].addr), 0, windows[i].len);
}

/* ---- the pseudo-random fixtures ---- */
static uint32_t rng;
static uint32_t rnd32(void) { rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5; return rng; }
static int rnd(int n) { return (int)(rnd32() % (uint32_t)n); }       /* 0 .. n-1 */
static int range(int lo, int hi) { return lo + rnd(hi - lo + 1); }   /* lo .. hi */
static int chance(int percent) { return rnd(100) < percent; }
static void fill_random(uint32_t addr, unsigned len) { unsigned i; for (i = 0; i < len; i++) ram(addr)[i] = (unsigned char)rnd32(); }

enum { F_RANGE, F_CLAMP, F_PUSH, F_FOLLOW, NFUNC };
static const char *const fname[NFUNC] = {
    "init_sprite_position_wide", "clamp_pos_x_to_bounds_and_push_other", "push_both_objects_inside_bounds", "update_camera_x_from_player_span",
};
static void (*override[NFUNC])(void);   /* the overrides by index, found by name in the table */
static void (*game_c[NFUNC])(void);

static int s16v(int v) { return (int16_t)v; }

/* a position that puts a fighter's edge within a pixel or two of a wall, or anywhere around the screen */
static int pick_x(int bm, int hw)
{
    switch (rnd(6)) {
    case 0: return bm + range(-2, 2) + hw;                 /* the left edge at the 384 view's left edge */
    case 1: return bm + 0x180 + range(-2, 2) - hw;         /* the right edge at its right edge */
    default: return range(bm - 150, bm + 550);
    }
}

/* one fighter: the fields that the four functions read, random where they decide a branch and random bytes elsewhere */
static void make_fighter(uint32_t o, uint32_t other, int bm)
{
    int hw = chance(3) ? (int)(rnd32() & 0xffff) : range(0, 80);
    fill_random(o, OBJECT_SIZE);
    wr32(o + offsetof(Object, other), other);
    wr16(o + offsetof(Object, half_width), hw);
    wr16(o + offsetof(Object, pos_x), chance(3) ? (int)(rnd32() & 0xffff) : pick_x(bm, s16v(hw)));
    wr16(o + offsetof(Object, health), chance(70) ? range(1, 1000) : -range(1, 1000));
    ram(o)[offsetof(Object, state_0)] = chance(60) ? 1 : (unsigned char)rnd32();
    ram(o)[offsetof(Object, state_0) + 1] = chance(60) ? 1 : (unsigned char)rnd32();
    ram(o)[offsetof(Object, field_241)] = chance(60) ? 0 : (unsigned char)range(1, 255);
    ram(o)[offsetof(Object, active_state)] = chance(70) ? 0 : (unsigned char)range(1, 255);
    ram(o)[offsetof(Object, flag_166_set)] = chance(60) ? 0 : (unsigned char)range(1, 255);
    ram(o)[offsetof(Object, status_flag)] = (unsigned char)((rnd32() & 0x7f) | (chance(30) ? 0x80 : 0));
    ram(o)[offsetof(Object, field_261)] = chance(60) ? 0 : (unsigned char)range(1, 255);
}

/* the camera's record: the position and range the follower and its callees read, and a random record elsewhere */
static void make_camera(uint32_t cam, int bm)
{
    fill_random(cam, LAYER_SIZE);
    wr16(cam + offsetof(Cam, pos_x), chance(80) ? range(256, 640) : range(bm - 100, bm + 100));
    wr16(cam + offsetof(Cam, min_x), range(200, 500));
    wr16(cam + offsetof(Cam, max_x), range(400, 700));
    wr16(cam + offsetof(Cam, base_x), range(-300, 300));
}

/* the fixture `n` of function `f`: the memory as the game would hold it before the call; returns the argument (an object or a sprite) */
static uint32_t setup(int f, unsigned n, int *x_out, int *y_out)
{
    int bm = range(200, 700);
    uint32_t arg = 0;
    rng = 0x9e3779b9u ^ (uint32_t)(f + 1) * 0x85ebca6bu ^ (n + 1) * 0xc2b2ae35u;
    rnd32(); rnd32();
    clear_windows();
    fill_random(0x80190400u, 0x80);                           /* ref_first, ref_second, ref_other and what is around them */
    fill_random(0x801aa5e0u, 0x10);                           /* box_margin and its neighbours */
    wr16(BOX_MARGIN, bm);
    make_fighter(PLAYER_LEFT, PLAYER_RIGHT, bm);
    make_fighter(PLAYER_RIGHT, PLAYER_LEFT, bm);
    make_camera(MAIN_LAYER, bm);
    wr16(BOX_MARGIN, bm);                                     /* box_margin lies inside main_layer: the camera's fill wrote over it */
    fill_random(OTHER_SPRITE, LAYER_SIZE);
    switch (f) {
    case F_RANGE:
        arg = chance(60) ? MAIN_LAYER : OTHER_SPRITE;
        *x_out = chance(70) ? range(0, 1023) : range(-3000, 3000);
        *y_out = chance(70) ? range(0, 1023) : range(-3000, 3000);
        if (chance(5)) { wr16(MAIN_LAYER + offsetof(Cam, min_x), 0); wr16(MAIN_LAYER + offsetof(Cam, max_x), 0); }
        break;
    case F_CLAMP: case F_PUSH:
        arg = chance(50) ? PLAYER_LEFT : PLAYER_RIGHT;
        if (chance(50)) {                                      /* the other fighter near, so that a push can happen */
            uint32_t other = arg == PLAYER_LEFT ? PLAYER_RIGHT : PLAYER_LEFT;
            wr16(other + offsetof(Object, pos_x), rd16(arg + offsetof(Object, pos_x)) + range(-300, 300));
        }
        break;
    case F_FOLLOW: {
        uint32_t l = PLAYER_LEFT, r = PLAYER_RIGHT;
        int hl = rdu16(l + offsetof(Object, half_width)), hr = rdu16(r + offsetof(Object, half_width));
        int lo, hi, span;
        arg = chance(80) ? MAIN_LAYER : OTHER_SPRITE;
        if (chance(30)) {                                      /* a span within a pixel or two of the width where the follower centres */
            hl = range(0, 80); hr = range(0, 80);
            wr16(l + offsetof(Object, half_width), hl); wr16(r + offsetof(Object, half_width), hr);
            lo = range(bm - 100, bm + 300);
            span = 0xc0 + range(-2, 2);
            hi = lo + span;
            wr16(l + offsetof(Object, pos_x), lo + hl); wr16(r + offsetof(Object, pos_x), hi - hr);
            ram(l)[offsetof(Object, status_flag)] &= 0x7f; ram(r)[offsetof(Object, status_flag)] &= 0x7f;
        }
        lo = s16v(rdu16(l + offsetof(Object, pos_x)) - hl);
        if (s16v(rdu16(r + offsetof(Object, pos_x)) - hr) < lo) lo = s16v(rdu16(r + offsetof(Object, pos_x)) - hr);
        hi = s16v(rdu16(l + offsetof(Object, pos_x)) + hl);
        if (s16v(rdu16(r + offsetof(Object, pos_x)) + hr) > hi) hi = s16v(rdu16(r + offsetof(Object, pos_x)) + hr);
        switch (rnd(5)) {                                      /* the camera at a pixel or two from where one of the follower's tests turns */
        case 0: wr16(arg + offsetof(Sprite, field_22), hi - 0x120 + range(-2, 2)); break;
        case 1: wr16(arg + offsetof(Sprite, field_22), lo - 0x60 + range(-2, 2)); break;
        case 2: wr16(arg + offsetof(Sprite, field_22), ((hi + lo) >> 1) - 0xc0 + range(-2, 2)); break;
        default: break;
        }
        break;
    }
    }
    return arg;
}

static void call(int f, void (*fn)(void), uint32_t arg, int x, int y)
{
    if (f == F_RANGE) port_call3(fn, arg, x, y);
    else port_call1(fn, arg);
}

struct setting { int widescreen, arcade_video, wide_ends; const char *name; };
static void apply(const struct setting *s)
{
    port_settings.widescreen = s->widescreen;
    port_settings.arcade_video = s->arcade_video;
    port_settings.wide_ends = s->wide_ends;
}

/* coverage of the branches, from what the game's C did in the fixtures */
static unsigned cover[16];
enum { C_CLAMP_L, C_CLAMP_R, C_CLAMP_NONE, C_CLAMP_PUSH, C_PUSH_L, C_PUSH_R, C_PUSH_NONE, C_PUSH_SWAP, C_PUSH_NOSWAP, C_FOLLOW_LEFT, C_FOLLOW_RIGHT, C_FOLLOW_SET };

/* The game's C and the override on identical memory, `count` fixtures of function f. 0 on success; else prints the difference. */
static int differential(int f, unsigned count, int e, const struct setting *s, int collect)
{
    static unsigned char before[SNAP_SIZE], want[SNAP_SIZE];
    unsigned n;
    for (n = 0; n < count; n++) {
        int x = 0, y = 0;
        uint32_t arg = setup(f, n, &x, &y), where;
        snapshot(before);
        port_wide_walls_extra = e;
        n_left = n_right = n_set = 0;
        call(f, game_c[f], arg, x, y);
        snapshot(want);
        if (collect) {
            uint32_t other = arg == PLAYER_LEFT ? PLAYER_RIGHT : PLAYER_LEFT;
            if (f == F_CLAMP) {
                unsigned flag = ram(arg)[offsetof(Object, bound_push_flag)];
                cover[flag == 1 ? C_CLAMP_L : flag == 2 ? C_CLAMP_R : C_CLAMP_NONE]++;
                if (rdu16(other + offsetof(Object, pos_x)) != (unsigned)(before[(other - PLAYER_LEFT) + 0x800 + offsetof(Object, pos_x)] | before[(other - PLAYER_LEFT) + 0x800 + offsetof(Object, pos_x) + 1] << 8)) cover[C_CLAMP_PUSH]++;
            } else if (f == F_PUSH) {
                unsigned flag = ram(other)[offsetof(Object, bound_push_flag)];
                cover[flag == 1 ? C_PUSH_L : flag == 2 ? C_PUSH_R : C_PUSH_NONE]++;
                cover[rd32(REF_FIRST) == other ? C_PUSH_SWAP : C_PUSH_NOSWAP]++;
            }
        }
        restore(before);
        apply(s);
        call(f, override[f], arg, x, y);
        if (collect && f == F_FOLLOW) { cover[C_FOLLOW_LEFT] += n_left != 0; cover[C_FOLLOW_RIGHT] += n_right != 0; cover[C_FOLLOW_SET] += n_set != 0; }
        where = first_difference(want);
        if (where) {
            printf("FAIL %s: fixture %u (extra %d, %s): memory differs from the game's C first at 0x%08x\n", fname[f], n, e, s->name, where);
            return 1;
        }
    }
    return 0;
}

/* ---- the hand-made fixtures at the extra of 64 ---- */
static int failed;
#define CHECK(cond) do { if (!(cond)) { printf("FAIL contract (line %d): %s\n", __LINE__, #cond); failed = 1; } } while (0)

static const struct setting narrowed = { 1, 1, PORT_WIDE_NARROWED, "narrowed" };

#define BM 400
static void scene(int hw_l, int x_l, int hw_r, int x_r)
{
    clear_windows();
    wr16(BOX_MARGIN, BM);
    wr32(PLAYER_LEFT + offsetof(Object, other), PLAYER_RIGHT);
    wr32(PLAYER_RIGHT + offsetof(Object, other), PLAYER_LEFT);
    wr16(PLAYER_LEFT + offsetof(Object, half_width), hw_l);
    wr16(PLAYER_RIGHT + offsetof(Object, half_width), hw_r);
    wr16(PLAYER_LEFT + offsetof(Object, pos_x), x_l);
    wr16(PLAYER_RIGHT + offsetof(Object, pos_x), x_r);
    wr16(PLAYER_LEFT + offsetof(Object, health), 100);
    wr16(PLAYER_RIGHT + offsetof(Object, health), 100);
}
static int px(uint32_t o) { return rd16(o + offsetof(Object, pos_x)); }
static int pflag(uint32_t o) { return ram(o)[offsetof(Object, bound_push_flag)]; }

static void contract(void)
{
    const int e = 64, hw = 20;
    apply(&narrowed);
    port_wide_walls_extra = e;

    /* clamp: the left wall is the 512 view's left edge, 64 left of box_margin */
    scene(hw, BM - 30 + hw, hw, BM + 200);                 /* the left edge 30 px left of the 384 view: inside the 512 */
    call(F_CLAMP, override[F_CLAMP], PLAYER_LEFT, 0, 0);
    CHECK(px(PLAYER_LEFT) == BM - 30 + hw && pflag(PLAYER_LEFT) == 0);
    scene(hw, BM - 70 + hw, hw, BM + 200);                 /* 70 px left: past the wall */
    call(F_CLAMP, override[F_CLAMP], PLAYER_LEFT, 0, 0);
    CHECK(px(PLAYER_LEFT) == BM - 64 + hw && pflag(PLAYER_LEFT) == 1);
    scene(hw, BM + 100, hw, BM + 0x180 + 30 - hw);         /* the right edge 30 px right of the 384 view */
    call(F_CLAMP, override[F_CLAMP], PLAYER_RIGHT, 0, 0);
    CHECK(px(PLAYER_RIGHT) == BM + 0x180 + 30 - hw && pflag(PLAYER_RIGHT) == 0);
    scene(hw, BM + 100, hw, BM + 0x180 + 70 - hw);         /* 70 px right: the right edge goes to box_margin + 384 + 64 */
    call(F_CLAMP, override[F_CLAMP], PLAYER_RIGHT, 0, 0);
    CHECK(px(PLAYER_RIGHT) == BM + 0x180 + 64 - hw && pflag(PLAYER_RIGHT) == 2);
    /* the game's own C moves the 30 px fighter: the contract differs from it */
    scene(hw, BM - 30 + hw, hw, BM + 200);
    call(F_CLAMP, game_c[F_CLAMP], PLAYER_LEFT, 0, 0);
    CHECK(px(PLAYER_LEFT) == BM + hw);
    /* a fighter pressed against the wall pushes the other by the same amount (state 0x101, close, not blocked) */
    scene(hw, BM + 0x180 + 70 - hw, hw, BM + 0x180 + 40 - hw);
    ram(PLAYER_LEFT)[offsetof(Object, state_0)] = 1; ram(PLAYER_LEFT)[offsetof(Object, state_0) + 1] = 1;
    call(F_CLAMP, override[F_CLAMP], PLAYER_LEFT, 0, 0);
    CHECK(px(PLAYER_LEFT) == BM + 0x180 + 64 - hw && px(PLAYER_RIGHT) == BM + 0x180 + 40 - hw - 6 && pflag(PLAYER_LEFT) == 2);
    CHECK(rd32(REF_OTHER) == PLAYER_RIGHT);

    /* push_both: the lower of the two against the left wall, the higher against the right */
    scene(hw, BM - 30 + hw, hw, BM + 200);
    call(F_PUSH, override[F_PUSH], PLAYER_LEFT, 0, 0);
    CHECK(px(PLAYER_LEFT) == BM - 30 + hw && px(PLAYER_RIGHT) == BM + 200 && pflag(PLAYER_RIGHT) == 0);
    scene(hw, BM - 70 + hw, hw, BM + 200);
    call(F_PUSH, override[F_PUSH], PLAYER_LEFT, 0, 0);
    CHECK(px(PLAYER_LEFT) == BM - 64 + hw && px(PLAYER_RIGHT) == BM + 200 + 6 && pflag(PLAYER_RIGHT) == 1);
    CHECK(rd32(REF_FIRST) == PLAYER_LEFT && rd32(REF_SECOND) == PLAYER_RIGHT && rd32(REF_OTHER) == PLAYER_RIGHT);
    scene(hw, BM + 100, hw, BM + 0x180 + 30 - hw);
    call(F_PUSH, override[F_PUSH], PLAYER_LEFT, 0, 0);
    CHECK(px(PLAYER_LEFT) == BM + 100 && px(PLAYER_RIGHT) == BM + 0x180 + 30 - hw && pflag(PLAYER_RIGHT) == 0);
    scene(hw, BM + 100, hw, BM + 0x180 + 70 - hw);
    call(F_PUSH, override[F_PUSH], PLAYER_LEFT, 0, 0);
    CHECK(px(PLAYER_RIGHT) == BM + 0x180 + 64 - hw && px(PLAYER_LEFT) == BM + 100 - 6 && pflag(PLAYER_RIGHT) == 2);
    scene(hw, BM + 100, hw, BM + 0x180 + 70 - hw);          /* the same called with the other object: the same result */
    call(F_PUSH, override[F_PUSH], PLAYER_RIGHT, 0, 0);
    CHECK(px(PLAYER_RIGHT) == BM + 0x180 + 64 - hw && px(PLAYER_LEFT) == BM + 100 - 6 && pflag(PLAYER_LEFT) == 2);

    /* init_sprite_position_wide: the camera's range cut by 64 at each end, for main_layer only */
    clear_windows();
    call(F_RANGE, override[F_RANGE], MAIN_LAYER, 300, 200);
    CHECK(rd16(MAIN_LAYER + offsetof(Cam, min_x)) == 0x140 && rd16(MAIN_LAYER + offsetof(Cam, max_x)) == 0x240);
    clear_windows();
    call(F_RANGE, override[F_RANGE], OTHER_SPRITE, 300, 200);
    CHECK(rd16(OTHER_SPRITE + offsetof(Cam, min_x)) == 0x100 && rd16(OTHER_SPRITE + offsetof(Cam, max_x)) == 0x280);
    CHECK(rd16(MAIN_LAYER + offsetof(Cam, min_x)) == 0);

    /* update_camera_x_from_player_span: a span of 250 follows the edges, one of 330 centres (the span where it centres is 320) */
    {
        const int cam = 300, lo = 500;
        clear_windows();
        scene(0, lo, 0, lo + 250);                          /* span 250 (192 .. 320): hi = 750 */
        wr16(MAIN_LAYER + offsetof(Sprite, field_22), cam);
        n_left = n_right = n_set = 0;
        call(F_FOLLOW, override[F_FOLLOW], MAIN_LAYER, 0, 0);
        CHECK(last_kind == 'r' && last_arg == lo + 250 - cam - (0x120 + 64));          /* the right edge's test, not the middle's */
        scene(0, lo, 0, lo + 330);                          /* span 330: the middle, 665 */
        wr16(MAIN_LAYER + offsetof(Sprite, field_22), cam);
        call(F_FOLLOW, override[F_FOLLOW], MAIN_LAYER, 0, 0);
        CHECK(last_kind == 'r' && last_arg == (lo + lo + 330) / 2 - cam - 0xc0);
        scene(0, lo, 0, lo + 319);                          /* 319: still the edges; 320: the middle */
        wr16(MAIN_LAYER + offsetof(Sprite, field_22), cam);
        call(F_FOLLOW, override[F_FOLLOW], MAIN_LAYER, 0, 0);
        CHECK(last_arg == lo + 319 - cam - (0x120 + 64));
        scene(0, lo, 0, lo + 320);
        wr16(MAIN_LAYER + offsetof(Sprite, field_22), cam);
        call(F_FOLLOW, override[F_FOLLOW], MAIN_LAYER, 0, 0);
        CHECK(last_arg == (lo + lo + 320) / 2 - cam - 0xc0);
        /* the left edge: the camera follows left when the span's left end is 32 or less right of it, else it holds still */
        scene(0, 520, 0, 770);
        wr16(MAIN_LAYER + offsetof(Sprite, field_22), 488);
        call(F_FOLLOW, override[F_FOLLOW], MAIN_LAYER, 0, 0);
        CHECK(last_kind == 'l' && last_arg == 0);
        scene(0, 520, 0, 770);
        wr16(MAIN_LAYER + offsetof(Sprite, field_22), 487);
        call(F_FOLLOW, override[F_FOLLOW], MAIN_LAYER, 0, 0);
        CHECK(last_kind == 's');
    }
}

int main(void)
{
    static const struct setting calls_through[] = {
        { 1, 1, PORT_WIDE_CENTRED, "wide_ends centred" }, { 1, 1, PORT_WIDE_ANCHORED, "wide_ends anchored" },
        { 0, 1, PORT_WIDE_NARROWED, "widescreen off" }, { 1, 0, PORT_WIDE_NARROWED, "arcade video off" },
    };
    const unsigned FIXTURES = 4000, CONTROL = 1000;
    unsigned i, f;
    void *p = mmap((void *)(uintptr_t)REGION, REGION_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    if (p != (void *)(uintptr_t)REGION) { puts("setup: cannot map the game's addresses"); return 2; }
    for (i = 0; port_wide_wall_overrides[i].name; i++)
        for (f = 0; f < NFUNC; f++)
            if (!strcmp(port_wide_wall_overrides[i].name, fname[f])) override[f] = (void (*)(void))port_wide_wall_overrides[i].host;
    game_c[F_RANGE] = (void (*)(void))init_sprite_position_wide;
    game_c[F_CLAMP] = (void (*)(void))clamp_pos_x_to_bounds_and_push_other;
    game_c[F_PUSH] = (void (*)(void))push_both_objects_inside_bounds;
    game_c[F_FOLLOW] = (void (*)(void))update_camera_x_from_player_span;
    for (f = 0; f < NFUNC; f++) if (!override[f]) { printf("setup: no override of %s in the table\n", fname[f]); return 2; }

    /* 1. the extra of 0: equal to the game's C */
    for (f = 0; f < NFUNC; f++) {
        if (differential((int)f, FIXTURES, 0, &narrowed, 1)) return 1;
    }
    {
        static const char *const names[] = { "clamp left wall", "clamp right wall", "clamp no wall", "clamp pushes the other", "push_both left", "push_both right",
                                             "push_both none", "push_both swapped", "push_both unswapped", "follow left", "follow right", "follow set" };
        for (i = 0; i < sizeof names / sizeof *names; i++)
            if (!cover[i]) { printf("FAIL coverage: the fixtures never reach '%s'\n", names[i]); return 1; }
    }
    /* 2. the extra of 64 */
    contract();
    if (failed) return 1;
    /* 3. any other setting: the game's C, even at 64 */
    for (i = 0; i < sizeof calls_through / sizeof *calls_through; i++)
        for (f = 0; f < NFUNC; f++) {
            if (differential((int)f, CONTROL, 64, &calls_through[i], 0)) return 1;
            }
    /* the control is not empty: with the walls on, the extra of 64 does differ from the game's C */
    {
        unsigned differs = 0;
        static unsigned char before[SNAP_SIZE], want[SNAP_SIZE];
        for (f = 0; f < NFUNC; f++) {
            unsigned n;
            for (n = 0; n < 200 && f != F_RANGE; n++) {
                int x = 0, y = 0;
                uint32_t arg = setup((int)f, n, &x, &y);
                snapshot(before);
                call((int)f, game_c[f], arg, x, y);
                snapshot(want);
                restore(before);
                apply(&narrowed);
                port_wide_walls_extra = 64;
                call((int)f, override[f], arg, x, y);
                if (first_difference(want)) differs++;
            }
        }
        if (!differs) { puts("FAIL control: the extra of 64 never differs from the game's C"); return 1; }
    }
    printf("wide walls ok: %u functions x %u fixtures at extra 0 equal to the game's C, the contract at 64, %u settings x %u fixtures call through\n",
           (unsigned)NFUNC, FIXTURES, (unsigned)(sizeof calls_through / sizeof *calls_through), CONTROL);
    return 0;
}
