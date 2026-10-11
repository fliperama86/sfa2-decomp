/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80019a74_slot28(Object *obj);

void func_800199dc_slot28(Object *obj) {
    int sum;
    ((Slot28Obj *)obj)->field_14 -= obj->vel_y;
    sum = obj->vel_y + obj->accel_y;
    obj->vel_y = sum;
    if (((Slot28Obj *)obj)->field_47 != 0) {
        if (sum >= 0) {
            ((Slot28Obj *)obj)->field_47 = 0;
            func_80019a74_slot28(obj);
        }
    } else if (sum < 0) {
        ((Slot28Obj *)obj)->field_47 = 0xff;
        func_80019a74_slot28(obj);
        obj->vel_y = -obj->vel_y;
        obj->accel_y = -obj->accel_y;
    }
}

void func_80019a74_slot28(Object *obj) {
    if (obj->field_03 == 4) {
        obj->vel_y = 0x14000;
    } else {
        obj->vel_y = 0x12000;
    }
    obj->accel_y = -0x1000;
}
