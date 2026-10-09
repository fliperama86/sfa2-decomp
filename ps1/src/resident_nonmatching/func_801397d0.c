/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs in register allocation only (the eight box values
 * sit in other registers). The exact owner of the bytes in the PS1 build
 * stays the raw bytes of the original; the build does not use this file.
 * The differential test next to it (func_801397d0.py, run with difftest.py)
 * compares this C with the original code on random inputs of the contract
 * below.
 *
 * What it does (inferred, not an original name): tests whether the box c,
 * placed at object b's position, is separated from a second box placed at
 * object a's position, and publishes the placement it used. The second box
 * comes from one of three places: when a->field_08 is 0, the entry of a's
 * table a->wide_boxes selected by a->frame->active (halfword fields, 0x20
 * bytes each); otherwise, when b->field_08 is 0, the box that the pointer
 * ref_first.p addresses, with halfword fields; otherwise the same pointer,
 * read as a box whose width and height are bytes. Box c has a halfword x
 * and y and byte width and height. When a's field_0b is not 0 the second
 * box x is negated; when b's is not 0, c's x is negated. The x test runs
 * first: the boxes are apart in x when the 16-bit distance of the two
 * placed x values exceeds the sum of the widths (compared as signed 16-bit
 * values), and 1 is returned without looking at y. Otherwise the same test
 * on y, with the heights, decides the result. All sums and differences are
 * taken modulo 2^16.
 *
 * Contract:
 *   Arguments: a0 = object a (the prototype types it s32; the code uses it
 *     as an Object pointer), a1 = object b, a2 = pointer to box c. Result
 *     in v0: 1 when apart, 0 when overlapping.
 *   Reads: both objects' field_08, field_0b, pos_x, pos_y; a's frame
 *     (active) and wide_boxes; ref_first.p and the box it points at; box c.
 *   Writes: data_80188ed0.out_3c, out_40 (always: the placed x values of a
 *     and b), and out_44, out_48 (only when the x test did not return: the
 *     placed y values minus the box y values). These are the halfwords at
 *     0x80188f0c to 0x80188f18.
 *   Aliasing: the objects, boxes, frame record and the box tables are
 *     distinct blocks; a and b are distinct.
 *   Excluded inputs: none. Every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801397d0(s32 a, Object *b, Box6 *c) {
    Object *self;
    Box32 *wide;
    Box6 *narrow;
    int ox;
    int oy;
    int ow;
    int oh;
    u16 ax;
    u16 bx;
    u16 ay;
    u16 by;
    s16 distance;

    self = (Object *)a;
    if (self->field_08 == 0) {
        wide = self->wide_boxes + self->frame->active;
        ox = wide->origin & 0xffff;
        oy = wide->field_02;
        ow = wide->endpoint & 0xffff;
        oh = wide->field_06;
    } else if (b->field_08 == 0) {
        wide = (Box32 *)ref_first.p;
        ox = wide->origin & 0xffff;
        oy = wide->field_02;
        ow = wide->endpoint & 0xffff;
        oh = wide->field_06;
    } else {
        narrow = (Box6 *)ref_first.p;
        ox = narrow->origin & 0xffff;
        oy = narrow->field_02;
        ow = narrow->extent;
        oh = narrow->field_05;
    }
    if (self->field_0b != 0) ox = -ox;
    bx = c->origin & 0xffff;
    if (b->field_0b != 0) bx = -bx;

    ax = ox + self->pos_x;
    bx = bx + b->pos_x;
    data_80188ed0.out_3c = ax;
    data_80188ed0.out_40 = bx;
    distance = bx - ax;
    if (distance < 0) distance = -distance;
    if ((s16)(c->extent + ow) < distance) return 1;

    ay = self->pos_y - oy;
    by = b->pos_y - c->field_02;
    data_80188ed0.out_44 = ay;
    data_80188ed0.out_48 = by;
    distance = by - ay;
    if (distance < 0) distance = -distance;
    return (s16)(c->field_05 + oh) < distance;
}
