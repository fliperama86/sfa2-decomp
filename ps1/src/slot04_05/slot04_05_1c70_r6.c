/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c1528_slot04_05[];

void func_801b22e0_slot04_05(Object *obj) {
    u16 i = 0;

    func_80130efc(obj);
    if (*(u8 *)&obj->step_flags == 0) {
        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c1528_slot04_05[i];
    }
}

void func_801b2370_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags == 1) {
        obj->field_45 = 1;
        obj->state_3++;
    }
    func_80130efc(obj);
}

void func_801b23b0_slot04_05(Object *obj) {
    if (obj->vel_y < 0) {
        obj->accel_y = obj->accel_y - 0x2000;
    }
    if ((u8)func_80130184(obj) != 0) {
        if (*(u8 *)&obj->step_flags != 2) {
            func_80130efc(obj);
        }
    } else {
        obj->state_3++;
        obj->pos_y = obj->field_70;
        obj->field_45 = 0;
        if (obj->field_0b != 0) {
            obj->vel_x = 0x70000;
        } else {
            obj->vel_x = -0x70000;
        }
        obj->vel_y = 0x30000;
        obj->accel_x = 0;
        obj->accel_y = -0x6000;
    }
}
