/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8014e25c(Object *object) {
    Object *other;
    FrameRecord *frame;
    u8 a, b, c;
    if (!func_8014e310(object)) return 0;
    other = object->other;
    if (other->active_state != 0) return 0;
    if (object->compare_limit >= 0x30) return 0;
    if (other->active_flag_3 == 0) return 0;
    if (*(u16 *)&other->state_0 != 1) return 0;
    frame = other->frame;
    b = frame->box_b;
    a = frame->box_a;
    c = frame->box_c;
    if (a != 0 || b != 0 || c != 0) return 1;
    return 0;
}
