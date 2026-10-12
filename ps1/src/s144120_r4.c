/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void func_80144544(Object *object) {
    func_80144220(object);
    object->countdown = (s16)object->countdown - 1;
    if (object->countdown & 0x80) {
        func_80144e90(object);
    }
}

void func_80144590(Object *object) {
    handlers_b61c[object->state_2](object);
    advance_sequence_checked(object);
    func_80120028(object);
}

void func_801445ec(Object *object) {
    object->pos_x = 0x1c0;
    object->pos_y = 0x70;
    object->countdown = 0x1f;
    object->vel_x = -0x20000;
    object->accel_x = -0x8000;
    object->state_2 = object->state_2 + 1;
    func_80144ee4(object, 0xa);
}

void func_80144640(Object *object) {
    func_80144220(object);
    object->countdown = (s16)object->countdown - 1;
    if (object->countdown & 0x80) {
        object->countdown = 0xf;
        object->accel_x = 0x40000;
        object->state_2 = object->state_2 + 1;
    }
}

void func_8014469c(Object *object) {
    func_80144220(object);
    object->countdown = (s16)object->countdown - 1;
    if (object->countdown & 0x80) {
        object->pos_x = 0xc0;
        object->state_2 = object->state_2 + 1;
    }
}
