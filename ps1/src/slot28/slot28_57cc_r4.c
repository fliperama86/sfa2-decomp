/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8002c7a4_slot28[];

void func_80015a70_slot28(Object *obj) {
    data_8002c7a4_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
}

void func_80015ac4_slot28(Object *obj) {
    if (--obj->vel_x == 0) {
        obj->vel_x = 0x60;
        obj->state_1++;
    }
    obj->field_48 = obj->field_48 ^ 1;
    obj->field_01 = 0;
    if (obj->field_48 != 0) {
        obj->field_01 = 1;
    }
}

void func_80015b1c_slot28(Object *obj) {
    if (--obj->vel_x == 0) {
        obj->vel_x = 0x20;
        obj->state_1++;
    }
    obj->field_01 = 1;
}
