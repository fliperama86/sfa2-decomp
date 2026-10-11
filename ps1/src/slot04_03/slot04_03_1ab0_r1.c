/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1ab0_slot04_03(Object *obj) {
    u16 t = obj->step_flags;

    if ((t & 0xff) != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        if (obj->field_0b != 0) {
            obj->vel_x = 0x50000;
            obj->accel_x = -0x2000;
        } else {
            obj->vel_x = -0x50000;
            obj->accel_x = 0x2000;
        }
    } else if ((t & 0xff00) == 0) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
    }
    func_80130efc(obj);
}

void func_801b1b4c_slot04_03(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    func_80130efc(obj);
}

void func_801b1bb8_slot04_03(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
    }
}
