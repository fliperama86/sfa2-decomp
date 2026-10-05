/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966[];

int func_8014e758(Object *object) {
    return func_8014e784(object->other) != 0;
}

u8 func_8014e784(Object *object) {
    FrameRecord *frame;
    u8 a, b, c;
    if (*(u16 *)&object->field_04 != 1) return 0;
    if (object->field_45 != 0) return 0;
    if (object->field_27b != 0) return 0;
    frame = object->frame;
    b = frame->box_b;
    a = frame->box_a;
    c = frame->box_c;
    if (a != 0 || b != 0 || c != 0) return 1;
    return 0;
}
