/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


int try_set_other_state_if_in_range(Object *object, s16 a, s16 b, s16 c, u16 d) {
    FrameRecord *frame;
    ref_other.p = object->other;
    if (ref_other.p->field_157 != 0) return 0;
    if (object->field_49 != 0) {
        if (*(u16 *)&object->state_0 == 1 && object->state_2 == 7) return apply_forced_state_if_other_in_limit(object, a, b) & 0xff;
        return 0;
    }
    if (ref_other.p->other_lock != 0) return 0;
    if (*(u16 *)&ref_other.p->state_0 != 1) return 0;
    frame = ref_other.p->frame;
    if (frame->box_a == 0 && frame->box_b == 0 && frame->box_c == 0) return 0;
    return check_range_and_set_other_state(object, a, b, c, (s16)d) & 0xff;
}

int check_hit_other_active_state_zero(Object *object, s16 a, s16 b, s16 c, u16 d) {
    FrameRecord *frame;
    ref_other.p = object->other;
    if (ref_other.p->active_state != 0) return 0;
    if (object->field_49 != 0) {
        if (*(u16 *)&object->state_0 == 1 && object->state_2 == 7) return apply_forced_state_if_other_in_limit(object, a, b) & 0xff;
        return 0;
    }
    if (ref_other.p->other_lock != 0) return 0;
    if (*(u16 *)&ref_other.p->state_0 != 1) return 0;
    frame = ref_other.p->frame;
    if (frame->box_a == 0 && frame->box_b == 0 && frame->box_c == 0) return 0;
    return check_range_and_set_other_state(object, a, b, c, (s16)d) & 0xff;
}

int check_hit_other_without_state_precheck(Object *object, s16 a, s16 b, s16 c, u16 d) {
    FrameRecord *frame;
    ref_other.p = object->other;
    if (object->field_49 != 0) {
        if (*(u16 *)&object->state_0 == 1 && object->state_2 == 7) return apply_forced_state_if_other_in_limit(object, a, b) & 0xff;
        return 0;
    }
    if (ref_other.p->other_lock != 0) return 0;
    if (*(u16 *)&ref_other.p->state_0 != 1) return 0;
    frame = ref_other.p->frame;
    if (frame->box_a == 0 && frame->box_b == 0 && frame->box_c == 0) return 0;
    return check_range_and_set_other_state(object, a, b, c, (s16)d) & 0xff;
}
