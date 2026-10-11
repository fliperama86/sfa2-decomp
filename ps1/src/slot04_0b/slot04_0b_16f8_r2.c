/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b18a8_slot04_0b(Object *obj) {
    if (obj->vel_y < 0) {
        if (obj->field_49 == 0) {
            obj->accel_y = -0x8000;
        } else {
            obj->accel_y = -0x10000;
        }
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
}
