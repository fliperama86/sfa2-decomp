/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8013fd98(Object *object, s16 a, s16 b, s16 c, u16 d) {
    FrameRecord *frame;
    ref_other.p = object->other;
    frame = ref_other.p->frame;
    if (frame->field_06 | ref_other.p->field_27b | ref_other.p->field_163) return 0;
    if (frame->box_a == 0 && frame->box_b == 0 && frame->box_c == 0) return 0;
    return func_801400fc(object, a, b, c, (s16)d) & 0xff;
}
