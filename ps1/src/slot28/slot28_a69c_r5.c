/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001add0_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->field_14 = obj->field_14 - o->vel_y;
    o->vel_y = o->vel_y + o->accel_y;
    if (obj->field_47 != 0) {
        if (o->vel_y >= 0) {
            o->vel_y = 0x8000;
            obj->field_47 = 0;
            o->accel_y = -0x400;
        }
    } else if (o->vel_y < 0) {
        o->vel_y = -0x8000;
        o->accel_y = 0x400;
        obj->field_47 = 0xff;
    }
    advance_sequence_checked(o);
    o->field_01 = 1;
}
