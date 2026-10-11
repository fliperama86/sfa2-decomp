/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2994_slot04_07(Object *obj);

void func_801b2300_slot04_07(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    o->state_3 = 2;
    o->field_46 = 8;
    o->accel_x = *(s32 *)&obj->field_1c0;
    func_801307e0(o, 0x24);
}

void func_801b2338_slot04_07(Object *obj) {
    obj->state_3 = 2;
    obj->field_157 = 1;
    if (obj->field_49 != 0) {
        func_801307e0(obj, 0x28);
    } else {
        func_801307e0(obj, 0x2c);
    }
}

void func_801b2378_slot04_07(Object *obj) {
    obj->state_3 = 3;
    obj->field_45 = 1;
    obj->vel_x = 0x70000;
    obj->accel_x = -0x1000;
    obj->vel_y = 0x50000;
    obj->accel_y = -0x8000;
    *(u16 *)&obj->pos_y -= 1;
    if (obj->field_49 != 0) {
        func_801307e0(obj, 0x27);
    } else {
        func_801307e0(obj, 0x2b);
    }
}

int func_801b23e4_slot04_07(Object *obj) {
    int t;

    if (obj->field_cd == 0) {
        return (s16)obj->field_21e < 0x20;
    }
    t = 0x30;
    if (obj->field_12a != 0) {
        t = 0x60;
    }
    return (s16)obj->field_21e < t;
}

void func_801b2428_slot04_07(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if ((t & 0xff) == 0) {
            func_801b2994_slot04_07(obj);
        }
        func_80130efc(obj);
    }
}
