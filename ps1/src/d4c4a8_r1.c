/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* The same distance as func_8014362c, for the object's partner (object->other)
   and a box chosen by the animation frame of the object that ref_second points
   at (inferred). The result goes to the global data_80189464. The partner's
   facing_side selects the mirrored arm. The distance: the object's x minus the sum
   of the partner's x and the box half-width (origin minus the extent byte,
   negated when the partner's facing_side is non-zero), as a 16-bit number made
   positive. */
void func_8014c4a8(Object *object) {
    Box6 *box;
    Object *other;
    int i;
    /* Stand-in, not recovered source: the locals neg, origin and e, each used again for later values, reproduce the original's choice of registers. Without it this function differs from the original in 12 instruction slots. */
    /* The later values are the mirrored extent, the partner's x, the object's x and the difference. The figure is for each of them in a local of its own, the copy below kept. Inferred: the original keeps those values in the same registers in turn. */
    int neg;
    int origin;
    int e;
    int sum;
    /* delta is a 16-bit local, the width of data_80189464: with an int local this function differs from the original in 3 instruction slots. */
    short delta;
    other = object->other;
    /* The index is read into a local before the box pointer is formed; with the box formed in one expression this function differs from the original in 15 instruction slots. */
    i = ref_second.p->frame->active;
    box = (Box6 *)ref_second.p->box_tables;
    box += i;
    e = box->extent;
    neg = -e;
    origin = (u16)box->origin;
    e = neg;
    if (other->facing_side != 0) {
        origin = -origin;
        e = -neg;
    }
    sum = origin + e;
    origin = (u16)other->pos_x;
    e = (u16)object->pos_x;
    neg = e - (sum + origin);
    /* Stand-in, not recovered source: this copy of the difference into origin reproduces the original's choice of registers. Without it this function differs from the original in 8 instruction slots. */
    /* The figure is for delta and the test read from neg. */
    origin = neg;
    delta = origin;
    if ((s16)origin < 0) {
        delta = -origin;
    }
    data_80189464 = delta;
}
