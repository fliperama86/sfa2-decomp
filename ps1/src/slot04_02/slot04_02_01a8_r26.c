/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3074_slot04_02(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0) {
        obj->state_3++;
    }
    func_80130efc(obj);
}

void func_801b30cc_slot04_02(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y < obj->field_70) {
        if ((u8)obj->step_flags != 0) {
            return;
        }
    } else {
        obj->field_159 = 0;
        obj->field_45 = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
    }
    func_80130efc(obj);
}
