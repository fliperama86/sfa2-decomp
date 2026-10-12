/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* What it does (inferred): walks the animation sequence of object b and
   tests, for each step whose frame has a box, whether that box (mirrored
   when b's facing_side is set) is within reach of object a's horizontal
   position. Returns 0 when b's current frame has bit 0x80 in field_0c, 2 at
   the first step whose box is within reach, 1 when the sequence ends. */
u8 func_8012fd80(Object *a, Object *b) {
    SequenceStep *s;
    Box32 *box;
    int active, off;
    s16 d, limit;

    s = b->sequence;
    if (b->frame->field_0c & 0x80) return 0;
    do {
    top:
        active = b->frames[s->frame_index].active;
        box = b->wide_boxes;
        if (active == 0) continue;
        box += active;
        /* origin is a signed halfword; without the cast this function differs from the original in 1 instruction line. */
        off = (u16)box->origin;
        if (b->facing_side) off = -off;
        d = off + b->pos_x - a->pos_x;
        limit = box->endpoint + 0x58;
        if (d < 0) d = -d;
        if (d > limit) {
            /* The arm leaves the loop for the one return 1 below: with a return 1 of its own here this
               function differs from the original in 11 instruction lines (8 instructions fewer). */
            if ((++s)->flags < 0) break;
            /* The step is taken once above and the walk resumes at the loop's top: written without the
               goto (a for or while loop with one step and test at the bottom, or the far arm turned round)
               this function differs from the original in 11 instruction lines or more. */
            goto top;
        }
        return 2;
    } while ((++s)->flags >= 0);
    return 1;
}
