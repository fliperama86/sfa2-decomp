/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b13e0_slot04_03(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        obj->field_45 = 1;
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        if (obj->field_49 != 0 && obj->vel_y > 0) {
            obj->accel_y = 0x10000;
        }
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac + obj->vel_y;
        obj->vel_y = obj->vel_y + obj->accel_y;
        if (obj->pos_y >= obj->field_70) {
            obj->state_3++;
            obj->field_45 = 0;
            obj->field_17b = 0;
            obj->pos_y = (u16)obj->field_70;
            func_801209c4(obj);
            func_801307e0(obj, 0x28);
            return;
        }
    }
    func_80130efc(obj);
}

void func_801b14b4_slot04_03(Object *obj) {
    func_80142adc(obj);
    if (!((s16)obj->step_flags & 0x8000)) {
        func_80130efc(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}
