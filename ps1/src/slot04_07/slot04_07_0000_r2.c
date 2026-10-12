/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0190_slot04_07(Object *obj) {
    obj->state_2 += 1;
    if (obj->facing_side == 0) {
        obj->vel_x = -0x40000;
    } else {
        obj->vel_x = 0x40000;
    }
    obj->vel_y = 0x10000;
    obj->accel_x = 0;
    obj->accel_y = -0x2a00;
    play_side_sound_at(obj, obj->side, 6);
}
