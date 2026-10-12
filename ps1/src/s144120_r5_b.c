/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801448c4(Object *object) {
    if ((s16)object->step_flags < 0) {
        object->pos_x = 0xc0;
        object->state_2 = object->state_2 + 1;
        func_80144ee4(object, 0x10);
        func_80144938(object);
        object->field_01 = 1;
        object->spawn_x = 0x80;
        object->ot_depth = 1;
    } else {
        object->field_01 = 2;
    }
}

void func_80144938(Object *object) {
    if (object->step_flags & 0x80) {
        object->field_46 = 7;
        object->vel_x = 0x8000;
        object->vel_y = -0x10000;
        object->state_2 = object->state_2 + 1;
        object->step_flags = object->step_flags & 0xff00;
    }
}

void func_80144980(Object *object) {
    func_80144244(object);
    object->field_46 = (s16)object->field_46 - 1;
    if (object->field_46 & 0x80) {
        object->state_2 = object->state_2 + 1;
    }
}
