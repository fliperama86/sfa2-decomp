/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80144bdc(Object *object) {
    int v;
    func_80144220(object);
    v = object->countdown - 1;
    object->countdown = v;
    if (v & 0x80) {
        func_801449fc(object);
    }
}

void func_80144c28(Object *object) {
    table_8017b668[object->state_2](object);
    advance_sequence_checked(object);
    func_80120028(object);
}

void func_80144c84(Object *object) {
    object->pos_x = -0x40;
    object->pos_y = 0x70;
    object->countdown = 0x1f;
    object->vel_x = 0x20000;
    object->accel_x = 0x8000;
    object->state_2++;
    func_80144ee4(object, 0xb);
}

void func_80144cd8(Object *object) {
    int v;
    func_80144220(object);
    v = object->countdown - 1;
    object->countdown = v;
    if (v & 0x80) {
        object->countdown = 0xf;
        object->accel_x = 0xfffc0000;
        object->state_2++;
    }
}

void func_80144d34(Object *object) {
    int v;
    func_80144220(object);
    v = object->countdown - 1;
    object->countdown = v;
    if (v & 0x80) {
        object->pos_x = 0xc0;
        object->state_2++;
    }
}
