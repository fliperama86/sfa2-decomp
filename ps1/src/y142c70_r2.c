/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80143184(Object *object) {
    int limit;
    Object *other = object->other;
    ref_other.p = other;
    if ((object->field_45 | ref_other.p->field_45) != 0) return;
    func_8014362c(object, other);
    limit = 0x40;
    if ((s16)object->field_21e > limit) return;
    if (ref_other.p->frame->active == 0 && ref_other.p->frame->box_c == 0 &&
        ref_other.p->frame->box_b == 0 && ref_other.p->frame->box_a == 0 &&
        ref_other.p->frame->field_04 == 0 && ref_other.p->frame->field_05 == 0 &&
        ref_other.p->frame->field_06 == 0) return;
    if (ref_other.p->frame->field_06 != 0) return;
    if (ref_other.p->field_27b != 0) return;
    if (ref_other.p->field_7e != 0) return;
    if (*(u16 *)&ref_other.p->field_04 == 0x801) return;
    if (*(u16 *)&ref_other.p->field_04 != 1) return;
    if (ref_other.p->field_06 == 7) return;
    if (ref_other.p->field_06 == 8) return;
    if (ref_other.p->side == 0) ref_other.p->sequence = seqs_68_left[3];
    else ref_other.p->sequence = seqs_118_right[3];
    ref_other.p->field_38 = ref_other.p->sequence->duration;
    ref_other.p->field_3a = ref_other.p->sequence->flags;
    ref_other.p->field_80 = 1;
    ref_other.p->frame = (FrameRecord *)((char *)ref_other.p->frames + (ref_other.p->sequence->frame_index << 4));
    ref_other.p->field_157 = 0;
}

int func_801433ac(Object *object) {
    int limit;
    Object *other = object->other;
    ref_other.p = other;
    if ((object->field_45 | ref_other.p->field_45) != 0) return 0;
    func_8014362c(object, other);
    limit = 0x40;
    if ((s16)object->field_21e > limit) return 0;
    if (ref_other.p->frame->active != 0 || ref_other.p->frame->box_c != 0 ||
        ref_other.p->frame->box_b != 0 || ref_other.p->frame->box_a != 0 ||
        ref_other.p->frame->field_04 != 0 || ref_other.p->frame->field_05 != 0) {
    } else {
out:
        return 0;
    }
    if (ref_other.p->frame->field_06 != 0) goto out;
    if (ref_other.p->field_27b != 0) goto out;
    if (ref_other.p->field_7e != 0) goto out;
    if (*(u16 *)&ref_other.p->field_04 == 0x801) goto out;
    if (*(u16 *)&ref_other.p->field_04 != 1) goto out;
    if (ref_other.p->field_06 == 7) goto out;
    if (ref_other.p->field_06 == 8) goto out;
    if (ref_other.p->side == 0) ref_other.p->sequence = seqs_60_left[0];
    else ref_other.p->sequence = seqs_110_right[0];
    ref_other.p->field_38 = ref_other.p->sequence->duration;
    ref_other.p->field_3a = ref_other.p->sequence->flags;
    ref_other.p->field_80 = 1;
    ref_other.p->frame = (FrameRecord *)((char *)ref_other.p->frames + (ref_other.p->sequence->frame_index << 4));
    ref_other.p->field_157 = 0;
    ref_other.p->field_04 = 1;
    ref_other.p->field_05 = 0;
    ref_other.p->field_06 = 0;
    ref_other.p->field_07 = 0;
    ref_other.p->field_207 = 0;
    ref_other.p->field_208 = 0;
    return 1;
}
