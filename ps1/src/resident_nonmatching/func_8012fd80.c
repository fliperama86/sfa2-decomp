/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice. The
 * build does not use this file. The differential test next to it
 * (difftest.py, with func_8012fd80.py) compares the behavior of this C with
 * the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): walks the animation
 * sequence of object b and tests, for each step whose frame has a box,
 * whether that box (mirrored when b's field_0b is set) lies within 0x58 of
 * object a's horizontal position, measured as a 16-bit distance. Returns 0
 * when b's current frame has bit 0x80 in its field_0c; 2 at the first step
 * whose box is within reach; 1 when the sequence ends (a step with a
 * negative flags word) without finding one.
 *
 * Contract:
 *   Arguments: a0 = object a, a1 = object b. Returns u8 (0, 1 or 2) in v0.
 *   Reads: b->frame (field_0c), b->sequence (the steps, from the first one
 *     on), b->frames (frame_index of a step selects a record; its active
 *     byte selects a box), b->wide_boxes (box index is the active byte,
 *     origin and endpoint halfwords of the box), b->field_0b, b->pos_x and
 *     a->pos_x. Writes nothing.
 *   Aliasing: a and b may be the same object or distinct; the test uses
 *     distinct blocks. No callee.
 *   Excluded: a sequence without a terminating step (negative flags)
 *     within the allocated steps; the original would read past it.
 *   Slots no input can reach: none known.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8012fd80(Object *a, Object *b) {
    SequenceStep *s;
    FrameRecord *frames;
    Box32 *box;
    int active, off, diff;
    s16 d, limit;

    s = b->sequence;
    if (b->frame->field_0c & 0x80) return 0;
    frames = b->frames;
    for (;;) {
        active = frames[s->frame_index].active;
        if (active != 0) {
            box = b->wide_boxes + active;
            off = (u16)box->origin;
            if (b->field_0b) off = -off;
            diff = off + (u16)b->pos_x - (u16)a->pos_x;
            limit = (u16)box->endpoint + 0x58;
            d = diff;
            if ((s16)diff < 0) d = -diff;
            if (!(limit < d)) return 2;
        }
        if ((++s)->flags < 0) return 1;
    }
}
