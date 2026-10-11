/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f40_slot04_01(Object *obj) {
    obj->state_3++;
    obj->pos_y_frac = 0;
    obj->field_17b = 1;
    obj->field_29c = 2;
    obj->field_45 = 1;
    if (obj->field_0b == 0) {
        *(s32 *)&obj->vel_x = -0x40000;
    } else {
        *(s32 *)&obj->vel_x = 0x40000;
    }
    func_801307e0(obj, 0x48);
}

void func_801b2f9c_slot04_01(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->step_flags == 0) {
        obj->state_3++;
        if (obj->field_0b == 0) {
            obj->pos_x -= 0x14;
        } else {
            obj->pos_x += 0x14;
        }
        func_80120554(obj, ((Slot04aObj *)obj)->field_a6, 0x317);
        func_80131d68(obj);
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
}

void func_801b3030_slot04_01(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->step_flags != 0) {
        obj->vel_y = 0x30000;
        obj->accel_y = -0x6000;
        obj->state_3++;
        if (obj->field_0b == 0) {
            obj->vel_x = 0x38000;
        } else {
            obj->vel_x = -0x38000;
        }
    }
}
