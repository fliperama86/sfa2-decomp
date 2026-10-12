/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* What it does (inferred, not original names): tests whether the object's
   "other" object (also stored in ref_other) is within horizontal reach, and
   if so calls func_8013fab4. Returns 0 when the other object or its frame
   record rules it out, or when the distance of the two x positions (with
   a half-width of the other's box) exceeds reach; -1 when the frame's
   handler_index is neither 0 nor 4 and field_d8 or field_157 of the other is
   set; otherwise 1. */
s16 func_8013fc18(Object *object, int arg_offset, int arg_reach) {
    s16 offset = arg_offset;
    s16 reach = arg_reach;
    Box6 *box;
    FrameRecord *frame;
    ObjectRef other_ref;
    s16 half_width;

    ref_other.p = object->other;
    other_ref = ref_other;
    frame = other_ref.p->frame;
    if (other_ref.p->other_state_code != 0)
        return 0;
    if (other_ref.p->other_lock != 0)
        return 0;
    /* a halfword at offset 4; state_0 is a byte: the plain spelling differs in 1 instruction line */
    if (*(u16 *)&other_ref.p->state_0 != 1)
        return 0;
    if (game_state.config->mode_flag_c != 0)
        return 0;
    if (other_ref.p->active_state != 0)
        return 0;
    if ((frame->box_a | frame->box_b | frame->box_c) == 0)
        return 0;
    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    do {
        if (object->facing_side != 0)
            offset = -offset;
        box = other_ref.p->unknown_148;
        half_width = box->origin - box->extent;
        offset = offset + object->pos_x;
        if (other_ref.p->saved_facing != 0)
            half_width = -half_width;
    } while (0);
    offset = offset - (half_width + other_ref.p->pos_x);
    if (offset < 0)
        offset = -offset;
    if (offset > reach)
        return 0;
    if (frame->handler_index != 0 && frame->handler_index != 4) {
        if (other_ref.p->field_d8 != 0)
            return -1;
        if (other_ref.p->field_157 != 0)
            return -1;
    }
    func_8013fab4(object);
    return 1;
}
