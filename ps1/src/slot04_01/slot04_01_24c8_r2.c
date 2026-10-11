/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b265c_slot04_01(Object *obj) {
    obj->field_249 = 2;
    if (obj->vel_x < 0) {
        obj->state_3++;
    } else {
        if (*(u8 *)&obj->step_flags == 0) {
            obj->field_45 = 1;
            *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
            obj->vel_y = obj->vel_y + obj->accel_y;
        }
        if (obj->field_0b == 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
    }
    update_sequence(obj);
}
