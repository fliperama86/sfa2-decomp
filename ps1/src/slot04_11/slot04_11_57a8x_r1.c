/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b57a8_slot04_11(Object *obj) {
    s32 a = obj->vel_x;

    if (obj->facing_side == 0) {
        a = -a;
    }
    *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    return obj->vel_y = obj->vel_y + obj->accel_y;
}
