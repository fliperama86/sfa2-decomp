/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int is_other_state_one_with_frame_boxes(Object *object) {
    return is_state_one_with_frame_boxes(object->other) != 0;
}

u8 is_state_one_with_frame_boxes(Object *object) {
    FrameRecord *frame;
    u8 a, b, c;
    if (*(u16 *)&object->state_0 != 1) return 0;
    if (object->active_state != 0) return 0;
    if (object->other_lock != 0) return 0;
    frame = object->frame;
    b = frame->box_b;
    a = frame->box_a;
    c = frame->box_c;
    if (a != 0 || b != 0 || c != 0) return 1;
    return 0;
}
