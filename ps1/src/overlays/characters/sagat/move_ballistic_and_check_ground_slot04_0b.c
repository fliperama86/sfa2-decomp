/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 move_ballistic_and_check_ground_slot04_0b(Object *o) {
    *(s32 *)&o->pos_x_frac += o->vel_x;
    *(s32 *)&o->pos_y_frac -= o->vel_y;
    o->vel_y += o->accel_y;
    return o->pos_y <= o->ground_y;
}
