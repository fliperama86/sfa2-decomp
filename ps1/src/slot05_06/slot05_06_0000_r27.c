/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801ca2e4_slot05_06(Object *obj) {
    obj->vel_x = 0x40000;
    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    obj->vel_y = 0x28000;
    obj->state_3++;
    obj->accel_x = 0;
    obj->accel_y = 0xffff6000;
    obj->field_45 = 1;
    if (obj->field_4b == 0) {
        obj->field_165 = 0xff;
    } else {
        obj->field_165 = 1;
    }
}

void func_801ca350_slot05_06(Object *obj) {
    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    obj->accel_x = 0;
    obj->state_3++;
    obj->vel_x = 0x20000;
    obj->vel_y = 0x78000;
    obj->accel_y = -0x6000;
    func_801307e0(obj, 0x1f);
}

void func_801ca3ac_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_1c3 != 0) {
        obj->field_1c3--;
    } else {
        if (obj->field_3a != 0) {
            o->field_45 = 1;
            obj->field_1c8 = 0;
            o->state_3++;
        }
        func_80130efc(o);
    }
}
