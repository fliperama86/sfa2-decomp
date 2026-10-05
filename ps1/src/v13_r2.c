/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d) {
    FrameRecord *frame;
    ref_other.p = object->other;
    if (ref_other.p->field_45 == 0) return 0;
    if (object->field_49 != 0) {
        if (*(u16 *)&object->field_04 != 1) return 0;
        if (object->field_06 != 7) return 0;
        return func_8013f474(object, a, b) & 0xff;
    }
    if (ref_other.p->field_27b != 0) return 0;
    if (*(u16 *)&ref_other.p->field_04 != 1) return 0;
    frame = ref_other.p->frame;
    if (frame->box_a == 0 && frame->box_b == 0 && frame->box_c == 0) return 0;
    return func_801400fc(object, a, b, c, (s16)d) & 0xff;
}
