/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/*
 * What it does (inferred, not original names): a sibling of func_8013fc18.
 * It tests whether the object's "other" object (field_40, also stored in
 * the global ref_other) is within horizontal reach of a, and if so returns
 * what func_8013fab4(object) returns, truncated to a byte; every refusal
 * returns 0. Refusals, in order: the object's field_49 is 0; the object's
 * field_7e is 0 and the configuration block's field_4e is set; the other
 * object's field_27b is set; its frame record has no box flag (box_a, box_b
 * and box_c all 0); the frame record's handler_index is not 0; the distance is
 * greater than limit; the other object's field_45 is set. The distance is
 * as in func_8013fc18, except that the box is the entry number box_index of
 * the other object's array of 6-byte boxes (its field_148): the object's x
 * plus a (negated when the object's field_0b is set), minus the other
 * object's x plus the box's half-width (origin minus extent, negated when
 * the other's field_158 is set), as a signed 16-bit number made positive.
 */
int func_8013f474(Object *object, int a, s16 limit) {
    FrameRecord *rec;
    Box6 *box;
    int d;
    int idx;
    int ext;
    int e;
    int diff;
    int pos;

    if (object->field_49 == 0) {
        return 0;
    }
    if (object->field_7e == 0 && game_state.config->field_4e != 0) {
        return 0;
    }
    ref_other.p = object->other;
    if (ref_other.p->field_27b != 0) {
        return 0;
    }
    rec = ref_other.p->frame;
    if (rec->box_a == 0 && rec->box_b == 0 && rec->box_c == 0) {
        return 0;
    }
    if (rec->handler_index != 0) {
        return 0;
    }
    if (object->field_0b != 0) {
        a = -a;
    }
    /* Two statements: the one-expression form differs from the original in 37 instruction slots. */
    box = (Box6 *)ref_other.p->unknown_148;
    idx = rec->box_index;
    box = &box[idx];
    /* The extent in its own local: without it this function differs from the original in 3 instruction slots. */
    ext = box->extent;
    d = (u16)box->origin - ext;
    /* The sum in its own local: without it this function differs from the original in 37 instruction slots. */
    pos = a + (u16)object->pos_x;
    e = d;
    if (ref_other.p->field_158 != 0) {
        e = -d;
    }
    /* Stand-in, not recovered source: the term that is added to the sum below, which is 0 for every value of d, reproduces the original's choice of registers. Without it this function differs from the original in 29 instruction slots. */
    diff = pos - (e + (u16)ref_other.p->pos_x + (int)(((u32)d << 24) & 0x100));
    a = diff;
    if ((s16)diff < 0) {
        a = -diff;
    }
    if (limit < (s16)a) {
        return 0;
    }
    /* Stand-in, not recovered source: the term that is ORed to field_45 below, which is 0 for every value of diff, reproduces the original's choice of registers. Without it this function differs from the original in 19 instruction slots. */
    if ((ref_other.p->field_45 | (int)(((u32)diff << 24) & 0x100)) != 0) {
        return 0;
    }
    return (u8)func_8013fab4(object);
}
