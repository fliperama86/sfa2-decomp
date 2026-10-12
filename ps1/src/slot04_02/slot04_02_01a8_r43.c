/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b55b8_slot04_02(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        if (obj->field_0b != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
        obj->vel_y = obj->vel_y + obj->accel_y;
        if (obj->pos_y >= obj->ground_y) {
            obj->field_45 = 0;
            obj->pos_y = obj->ground_y;
            func_801209c4(obj);
            func_801312b8(obj);
            return;
        }
    }
    update_sequence(obj);
}
