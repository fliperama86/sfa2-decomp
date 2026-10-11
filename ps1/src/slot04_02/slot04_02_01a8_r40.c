/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4e68_slot04_02(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
        obj->vel_y = obj->vel_y + obj->accel_y;
        if (obj->pos_y >= obj->field_70) {
            obj->state_3 = obj->state_3 + 1;
            obj->field_45 = 0;
            obj->pos_y = obj->field_70;
            func_801209c4(obj);
            func_801307e0(obj, 0x4b);
        }
    }
}
