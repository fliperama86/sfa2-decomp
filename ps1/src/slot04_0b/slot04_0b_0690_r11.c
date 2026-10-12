/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f74_slot04_0b(Object *obj) {
    if (obj->vel_x >= 0) {
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->facing_side == 0) {
            *(s32 *)&obj->pos_x_frac -= obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
        }
        obj->vel_x += obj->accel_x;
    } else {
        obj->vel_x = 0;
        obj->accel_x = 0;
        obj->state_3++;
    }
    update_sequence(obj);
}
