/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);
void func_801b2994_slot04_07(Object *obj);

void func_801b279c_slot04_07(Object *obj) {
    s16 t;

    func_80130efc(obj);
    t = obj->step_flags;
    if (t & 0x8000) {
        obj->state_3 = 5;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x4c);
    } else if (t != 0x3ff) {
        if ((t & 0xff) != 0) {
            obj->step_flags = t & 0xff00;
            obj->vel_x = 0x80000;
            obj->accel_x = -0x8000;
        }
        func_801b2994_slot04_07(obj);
    } else {
        obj->vel_y = 0x90000;
        obj->accel_y = 0xffff0000;
        obj->field_45 = 1;
        obj->state_3++;
        obj->accel_x = 0;
        if (obj->field_0b != 0) {
            obj->vel_x = 0xfffc8000;
        } else {
            obj->vel_x = 0x38000;
        }
    }
}

void func_801b2874_slot04_07(Object *obj) {
    if (func_801b41c4_slot04_07(obj) >= 0 || obj->pos_y < obj->field_70) {
        func_80130efc(obj);
    } else {
        obj->pos_y = obj->field_70;
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->state_3 = 5;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x4c);
    }
}

void func_801b28f4_slot04_07(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->vel_x = 0xe0000;
        obj->accel_x = -0x6000;
        obj->state_3++;
    }
    func_80130efc(obj);
}
