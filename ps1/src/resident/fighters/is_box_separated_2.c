/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/*
 * What it does (inferred, not an original name): tests whether the box c,
 * placed at object b's position, is separated from a second box placed at
 * object a's position, and publishes the placement it used. The second box
 * comes from one of three places: when a->mode_id is 0, the entry of a's
 * table a->wide_boxes selected by a->frame->active (halfword fields, 0x20
 * bytes each); otherwise, when b->mode_id is 0, the box that the pointer
 * ref_first.p addresses, with halfword fields; otherwise the same pointer,
 * read as a box whose width and height are bytes. Box c has a halfword x
 * and y and byte width and height. When a's facing_side is not 0 the second
 * box x is negated; when b's is not 0, c's x is negated. The x test runs
 * first: the boxes are apart in x when the 16-bit distance of the two
 * placed x values exceeds the sum of the widths (compared as signed 16-bit
 * values), and 1 is returned without looking at y. Otherwise the same test
 * on y, with the heights, decides the result. All sums and differences are
 * taken modulo 2^16.
 */
u8 is_box_separated_2(s32 arg_a, Object *b, Box6 *c) {
    Object *a = (Object *)arg_a;
    int pw, ph, ow, oh, px, py, ox, oy;
    int ax, bx, ay, by;
    int dx, dy;
    s16 adx;

    if (a->mode_id == 0) {
        Box32 *w = a->wide_boxes + a->frame->active;
        ox = w->origin & 0xffff;
        ow = w->endpoint & 0xffff;
        oy = w->box_y;
        oh = w->box_h;
    } else if (b->mode_id == 0) {
        Box32 *w = (Box32 *)ref_first.p;
        ox = w->origin & 0xffff;
        ow = w->endpoint & 0xffff;
        oy = w->box_y;
        oh = w->box_h;
    } else {
        Box6 *r = (Box6 *)ref_first.p;
        ox = r->origin & 0xffff;
        ow = r->extent;
        oy = r->origin_y;
        oh = r->extent_y;
    }
    px = c->origin & 0xffff;
    py = c->origin_y;
    pw = c->extent;
    ph = c->extent_y;
    if (a->facing_side != 0) {
        ox = -ox;
    }
    if (b->facing_side != 0) {
        px = -px;
    }
    /* Stand-in, not recovered source: one stand-in at 2 places, the two terms that are added to the placed x of a below, each of which is 0 for every value of ox, reproduce the original's choice of registers. Without them this function differs from the original in 7 instruction slots. */
    ax = ox + (a->pos_x & 0xffff) + (int)(((u32)ox << 24) & 0x100) + (int)(((u32)ox << 23) & 0x100);
    bx = px + (b->pos_x & 0xffff);
    data_80188ed0.out_3c = ax;
    data_80188ed0.out_40 = bx;
    dx = bx - ax;
    adx = ((u32)dx << 16) >> 31 ? -dx : dx;
    if ((s16)(pw + ow) < (s16)adx) {
        return 1;
    }
    /* Stand-in, not recovered source: the term that is added to the placed y of a below, which is 0 for every value of oy, reproduces the original's choice of registers. Without it this function differs from the original in 6 instruction slots. */
    ay = (a->pos_y & 0xffff) - oy + (int)(((u32)oy << 24) & 0x100);
    /* Stand-in, not recovered source: the term that is added to the placed y of b below, which is 0 for every value of py, reproduces the original's choice of registers. Without it this function differs from the original in 8 instruction slots. */
    by = (b->pos_y & 0xffff) - py + (int)(((u32)py << 24) & 0x100);
    data_80188ed0.out_44 = ay;
    data_80188ed0.out_48 = by;
    dy = by - ay;
    /* One local (py) holds both the box y and the absolute y distance. Written with a separate local, this function differs from the original in 15 instruction slots. */
    py = ((u32)dy << 16) >> 31 ? -dy : dy;
    /* Stand-in, not recovered source: the term that is ORed to the absolute y distance below, which is 0 for every value of dy, reproduces the original's order of instructions. Without it this function differs from the original in 15 instruction slots (built size 340 bytes instead of 344). */
    /* The (int) cast is plain C; with u32 here the function differs from the original in 1 instruction slot. */
    return (s16)(ph + oh) < ((s16)py | (int)(((u32)dy << 24) & 0x100));
}
