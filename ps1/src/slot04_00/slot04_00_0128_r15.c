/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b16a0_slot04_00(Object *obj) {
    if (obj->field_49 != 0 && obj->vel_y < 0) {
        obj->accel_y = 0xffff0000;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
}
