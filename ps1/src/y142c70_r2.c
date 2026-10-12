/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void set_other_sequence_3_when_close(Object *object) {
    int limit;
    Object *other = object->other;
    ref_other.p = other;
    if ((object->active_state | ref_other.p->active_state) != 0) return;
    set_compare_limit_from_box_distance(object, other);
    limit = 0x40;
    if ((s16)object->compare_limit > limit) return;
    if (ref_other.p->frame->active == 0 && ref_other.p->frame->box_c == 0 &&
        ref_other.p->frame->box_b == 0 && ref_other.p->frame->box_a == 0 &&
        ref_other.p->frame->value_a == 0 && ref_other.p->frame->value_b == 0 &&
        ref_other.p->frame->handler_index == 0) return;
    if (ref_other.p->frame->handler_index != 0) return;
    if (ref_other.p->other_lock != 0) return;
    if (ref_other.p->field_7e != 0) return;
    if (*(u16 *)&ref_other.p->state_0 == 0x801) return;
    if (*(u16 *)&ref_other.p->state_0 != 1) return;
    if (ref_other.p->state_2 == 7) return;
    if (ref_other.p->state_2 == 8) return;
    if (ref_other.p->side == 0) ref_other.p->sequence = seqs_68_left[3];
    else ref_other.p->sequence = seqs_118_right[3];
    ref_other.p->step_timer = ref_other.p->sequence->duration;
    ref_other.p->step_flags = ref_other.p->sequence->flags;
    ref_other.p->sequence_changed_flag = 1;
    ref_other.p->frame = (FrameRecord *)((char *)ref_other.p->frames + (ref_other.p->sequence->frame_index << 4));
    ref_other.p->field_157 = 0;
}

int set_other_sequence_0_when_close(Object *object) {
    int limit;
    Object *other = object->other;
    ref_other.p = other;
    if ((object->active_state | ref_other.p->active_state) != 0) return 0;
    set_compare_limit_from_box_distance(object, other);
    limit = 0x40;
    if ((s16)object->compare_limit > limit) return 0;
    if (ref_other.p->frame->active != 0 || ref_other.p->frame->box_c != 0 ||
        ref_other.p->frame->box_b != 0 || ref_other.p->frame->box_a != 0 ||
        ref_other.p->frame->value_a != 0 || ref_other.p->frame->value_b != 0) {
    } else {
out:
        return 0;
    }
    if (ref_other.p->frame->handler_index != 0) goto out;
    if (ref_other.p->other_lock != 0) goto out;
    if (ref_other.p->field_7e != 0) goto out;
    if (*(u16 *)&ref_other.p->state_0 == 0x801) goto out;
    if (*(u16 *)&ref_other.p->state_0 != 1) goto out;
    if (ref_other.p->state_2 == 7) goto out;
    if (ref_other.p->state_2 == 8) goto out;
    if (ref_other.p->side == 0) ref_other.p->sequence = seqs_60_left[0];
    else ref_other.p->sequence = seqs_110_right[0];
    ref_other.p->step_timer = ref_other.p->sequence->duration;
    ref_other.p->step_flags = ref_other.p->sequence->flags;
    ref_other.p->sequence_changed_flag = 1;
    ref_other.p->frame = (FrameRecord *)((char *)ref_other.p->frames + (ref_other.p->sequence->frame_index << 4));
    ref_other.p->field_157 = 0;
    ref_other.p->state_0 = 1;
    ref_other.p->state_1 = 0;
    ref_other.p->state_2 = 0;
    ref_other.p->state_3 = 0;
    ref_other.p->field_207 = 0;
    ref_other.p->field_208 = 0;
    return 1;
}
