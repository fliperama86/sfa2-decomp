/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1bc0_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->field_249 = 5;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac & 0xffff0000;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}

void func_801b1c30_slot04_02(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3 = 1;
        obj->vel_x = 0x80000;
        obj->accel_x = -0x8000;
        obj->vel_y = 0x90000;
        obj->accel_y = -0x6000;
        func_801307e0(obj, 0x60);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            func_80146998(obj);
        }
        update_sequence(obj);
    }
}

void func_801b1cc0_slot04_02(Object *obj) {
    if (obj->field_49 != 0 && obj->vel_y < 0) {
        obj->accel_y = 0xffff0000;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
}
