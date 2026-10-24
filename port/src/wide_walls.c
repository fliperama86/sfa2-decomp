/* Widescreen with wide_ends = narrowed (port/widescreen.md, "The walls of the wide view"): the game's screen taken as the wide view's
 * 512 pixels in place of its 384, with the stage's walls where they are on the PS1.
 *
 * Every stage's camera range is 256..640 (the 384 view's left edge, init_sprite_position_wide), so the walls stand at 256 and 1024 of the
 * world: the fighters are held inside the camera's 384 (clamp_pos_x_to_bounds_and_push_other, push_both_objects_inside_bounds), and the
 * camera follows them with the 384's margins (update_camera_x_from_player_span). Here, with widescreen on and wide_ends narrowed:
 *
 * - the camera's range is cut by PORT_WIDE_EXTRA at each end (320..576), so the wide view (the camera's edge less 64, 512 wide) ends at the
 *   stage's ends: the walls of the world stay at 256 and 1024, and nothing past the stage's art is shown;
 * - the fighters are held inside the wide view (its edge, 512 wide) in place of the 384;
 * - the camera follows with the same margins from the wide view's edges (96 pixels), and centres the fighters from a span of 320 (512
 *   less both margins) in place of 192.
 *
 * The three are the game's C with those numbers (read from ps1/src/resident/graphics/init_sprite_position_wide.c,
 * resident/fighters/clamp_pos_x_to_bounds_and_push_other.c and resident/graphics/update_camera_x_from_player_span.c on 2026-10-24),
 * and with any other setting the game's own C runs. port/tools/wide_walls_test.c compares each with the game's C at an extra of 0. */
#include "port.h"
#include "settings.h"
#include "video.h"

#include <stdint.h>

#define MAIN_LAYER   0x801aa5d4u   /* main_layer, the camera's record */
#define BOX_MARGIN   0x801aa5e6u   /* box_margin[0]: the 384 view's left edge (u16) */
#define REF_OTHER    0x80190460u   /* ref_other, ref_first, ref_second: the game's pointers that the two clamps leave set */
#define REF_FIRST    0x8019040cu
#define REF_SECOND   0x80190410u
#define PLAYER_LEFT  0x80197fc0u   /* player_left; player_right follows it */
#define OBJECT_SIZE  0x394u

/* Object (ps1/src/types.fields) */
#define O_STATE_0      0x004u   /* u16 read of state_0 and the byte after it */
#define O_POS_X        0x012u   /* s16 */
#define O_OTHER        0x040u   /* Object * */
#define O_ACTIVE_STATE 0x045u
#define O_HEALTH       0x05cu   /* u16 */
#define O_STATUS_FLAG  0x073u
#define O_HALF_WIDTH   0x154u   /* u16 */
#define O_PUSH_FLAG    0x164u
#define O_FLAG_166     0x166u
#define O_FIELD_241    0x241u
#define O_FIELD_261    0x261u
/* Cam / Sprite */
#define C_POS_X        0x022u   /* Sprite.field_22, Cam.pos_x (s16) */
#define C_MIN_X        0x044u   /* Cam.min_x, Cam.max_x (s16): the range move_cam_left and move_cam_right clamp to */
#define C_MAX_X        0x046u

static port_fn range_c, clamp_c, push_c, follow_c, left_c, right_c, set_c;
static int said_range;

int port_wide_walls_extra = PORT_WIDE_EXTRA;   /* the test sets 0 to compare with the game's C */

static int narrowed(void)
{
    return port_settings.widescreen && port_settings.arcade_video && port_settings.wide_ends == PORT_WIDE_NARROWED;
}

static unsigned char *mem(uint32_t a, unsigned n) { return port_gs((const void *)(uintptr_t)a, n); }
static int s16_at(const unsigned char *p) { return (int16_t)(p[0] | p[1] << 8); }
static unsigned u16_at(const unsigned char *p) { return (unsigned)(p[0] | p[1] << 8); }
static uint32_t u32_at(const unsigned char *p) { return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24; }
static void put16(unsigned char *p, int v) { p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); }
static void put32(uint32_t a, uint32_t v)
{
    unsigned char *p = mem(a, 4);
    if (p) { p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); p[2] = (unsigned char)(v >> 16); p[3] = (unsigned char)(v >> 24); }
}

/* In place of init_sprite_position_wide(Sprite *sprite, int x, int y): the camera's start at a round, then main_layer's range cut. */
static void port_o_cam_range(uint32_t sprite, uint32_t x, uint32_t y)
{
    unsigned char *cam;
    int min, max, e = port_wide_walls_extra;
    if (!range_c) range_c = port_function_impl("init_sprite_position_wide");
    port_call3(range_c, sprite, x, y);
    if (!narrowed() || sprite != MAIN_LAYER || !(cam = mem(MAIN_LAYER, C_MAX_X + 2))) return;
    min = s16_at(cam + C_MIN_X);
    max = s16_at(cam + C_MAX_X);
    if (max - min < 2 * e) return;
    put16(cam + C_MIN_X, min + e);
    put16(cam + C_MAX_X, max - e);
    if (!said_range && e) {
        said_range = 1;
        printf("wide-video: the camera's range is %d..%d, the fighters held inside the 512 view: the walls stay at the stage's ends\n",
               min + e, max - e);
        fflush(stdout);
    }
}

/* The test of a push of the other fighter, as both clamps' C has it. */
static int may_push(const unsigned char *o, int dist, int limit)
{
    return u16_at(o + O_STATE_0) == 0x101 && o[O_FIELD_241] == 0 && o[O_ACTIVE_STATE] == 0 && dist < limit;
}

/* In place of clamp_pos_x_to_bounds_and_push_other(Object *object). */
static void port_o_clamp(uint32_t object)
{
    unsigned char *o, *p, *margin;
    uint32_t other;
    int limit, dist, d, span, x, left, e = port_wide_walls_extra;
    if (!clamp_c) clamp_c = port_function_impl("clamp_pos_x_to_bounds_and_push_other");
    if (!narrowed() || !(o = mem(object, OBJECT_SIZE)) || !(margin = mem(BOX_MARGIN, 2))) { port_call1(clamp_c, object); return; }
    o[O_PUSH_FLAG] = 0;
    other = u32_at(o + O_OTHER);
    put32(REF_OTHER, other);
    if (!(p = mem(other, OBJECT_SIZE))) return;
    limit = 0x100;
    if (s16_at(o + O_HEALTH) >= 0 && s16_at(p + O_HEALTH) >= 0) limit = 0x150;
    x = s16_at(o + O_POS_X);
    dist = s16_at(p + O_POS_X) - x;
    if (dist < 0) dist = -dist;
    span = (int16_t)u16_at(o + O_HALF_WIDTH);
    left = s16_at(margin) - e;
    d = x - span - left;
    if (d <= 0) {
        put16(o + O_POS_X, x - d);
        o[O_PUSH_FLAG] = 1;
        if (may_push(o, dist, limit)) goto push;
    } else {
        d += span * 2;
        d -= 0x180 + 2 * e;
        if (d >= 0) {
            put16(o + O_POS_X, x - d);
            o[O_PUSH_FLAG] = 2;
            if (may_push(o, dist, limit)) {
push:
                if (o[O_FLAG_166] == 0 && p[O_ACTIVE_STATE] == 0) put16(p + O_POS_X, s16_at(p + O_POS_X) - d);
            }
        }
    }
}

/* In place of push_both_objects_inside_bounds(Object *object). */
static void port_o_push_both(uint32_t object)
{
    unsigned char *o, *p, *first, *second, *margin;
    uint32_t other;
    int span, left, e = port_wide_walls_extra;
    int16_t d;
    uint16_t x;
    if (!push_c) push_c = port_function_impl("push_both_objects_inside_bounds");
    if (!narrowed() || !(o = mem(object, OBJECT_SIZE)) || !(margin = mem(BOX_MARGIN, 2))) { port_call1(push_c, object); return; }
    other = u32_at(o + O_OTHER);
    put32(REF_OTHER, other);
    put32(REF_FIRST, other);
    put32(REF_SECOND, object);
    if (!(p = mem(other, OBJECT_SIZE))) return;
    p[O_PUSH_FLAG] = 0;
    first = p; second = o;
    if (!(s16_at(p + O_POS_X) < s16_at(o + O_POS_X))) {
        put32(REF_FIRST, object);
        put32(REF_SECOND, other);
        first = o; second = p;
    }
    left = (int)u16_at(margin) - e;
    span = (int)u16_at(first + O_HALF_WIDTH);
    x = (uint16_t)u16_at(first + O_POS_X);
    d = (int16_t)(x - span - left);
    if (d < 0) {
        put16(o + O_POS_X, s16_at(o + O_POS_X) - d);
        put16(p + O_POS_X, s16_at(p + O_POS_X) - d);
        p[O_PUSH_FLAG] = 1;
    } else {
        x = (uint16_t)u16_at(second + O_POS_X);
        span = (int)u16_at(second + O_HALF_WIDTH);
        d = (int16_t)(x - left + span - (0x180 + 2 * e));
        if (d >= 0) {
            put16(o + O_POS_X, s16_at(o + O_POS_X) - d);
            put16(p + O_POS_X, s16_at(p + O_POS_X) - d);
            p[O_PUSH_FLAG] = 2;
        }
    }
}

/* In place of update_camera_x_from_player_span(Sprite *sprite): the span the players cover, then the camera moved toward it. */
static void port_o_follow(uint32_t sprite)
{
    unsigned char *s, *l, *r;
    uint16_t lo, hi, other, other2;
    int16_t amount;
    int e = port_wide_walls_extra;
    if (!follow_c) follow_c = port_function_impl("update_camera_x_from_player_span");
    if (!narrowed() || !(s = mem(sprite, C_POS_X + 2)) || !(l = mem(PLAYER_LEFT, 2 * OBJECT_SIZE))) { port_call1(follow_c, sprite); return; }
    if (!left_c) { left_c = port_function_impl("move_cam_left"); right_c = port_function_impl("move_cam_right"); set_c = port_function_impl("set_cam_pos_x"); }
    r = l + OBJECT_SIZE;
    if ((l[O_STATUS_FLAG] & 0x80) && l[O_FIELD_261] == 0) {
        lo = (uint16_t)u16_at(r + O_POS_X);
        hi = lo;
        lo = (uint16_t)(lo - u16_at(r + O_HALF_WIDTH));
        hi = (uint16_t)(hi + u16_at(r + O_HALF_WIDTH));
    } else if ((r[O_STATUS_FLAG] & 0x80) && r[O_FIELD_261] == 0) {
        lo = (uint16_t)u16_at(l + O_POS_X);
        hi = lo;
        lo = (uint16_t)(lo - u16_at(l + O_HALF_WIDTH));
        hi = (uint16_t)(hi + u16_at(l + O_HALF_WIDTH));
    } else {
        lo = (uint16_t)(u16_at(l + O_POS_X) - u16_at(l + O_HALF_WIDTH));
        other = (uint16_t)(u16_at(r + O_POS_X) - u16_at(r + O_HALF_WIDTH));
        if ((int16_t)other < (int16_t)lo) lo = other;
        hi = (uint16_t)(u16_at(l + O_POS_X) + u16_at(l + O_HALF_WIDTH));
        other2 = (uint16_t)(u16_at(r + O_POS_X) + u16_at(r + O_HALF_WIDTH));
        if ((int16_t)other2 > (int16_t)hi) hi = other2;
    }
    amount = (int16_t)(hi - lo);
    if (amount >= 0xc0 + 2 * e) {
        amount = (int16_t)(hi + lo);
        amount = (int16_t)((amount >> 1) - s16_at(s + C_POS_X) - 0xc0);
        if (amount < 0) port_call2(left_c, sprite, (int32_t)amount);
        else port_call2(right_c, sprite, (int32_t)amount);
    } else {
        amount = (int16_t)(hi - s16_at(s + C_POS_X) - (0x120 + e));
        if (amount >= 0) port_call2(right_c, sprite, (int32_t)amount);
        else {
            amount = (int16_t)(lo - s16_at(s + C_POS_X) - (0x60 - e));
            if (amount <= 0) port_call2(left_c, sprite, (int32_t)amount);
            else port_call2(set_c, sprite, (int32_t)s16_at(s + C_POS_X));
        }
    }
}

const struct port_override port_wide_wall_overrides[] = {
    { "init_sprite_position_wide", (void *)port_o_cam_range,
      "the camera's start at a round; with widescreen and wide_ends narrowed its range cut by 64 at each end", NULL },
    { "clamp_pos_x_to_bounds_and_push_other", (void *)port_o_clamp,
      "a fighter held inside the screen; with widescreen and wide_ends narrowed the 512 view in place of the 384", NULL },
    { "push_both_objects_inside_bounds", (void *)port_o_push_both,
      "both fighters held inside the screen; with widescreen and wide_ends narrowed the 512 view in place of the 384", NULL },
    { "update_camera_x_from_player_span", (void *)port_o_follow,
      "the camera's follow of the fighters; with widescreen and wide_ends narrowed the 512 view's margins", NULL },
    { NULL, NULL, NULL, NULL }
};
