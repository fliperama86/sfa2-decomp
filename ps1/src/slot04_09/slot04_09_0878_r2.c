/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5ba0_slot04_09(Object *obj);

void count_down_then_halve_velocities_2_slot04_09(Object *obj) {
    func_801b5ba0_slot04_09(obj);
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->vel_x >>= 1;
        obj->accel_x >>= 1;
        obj->vel_y >>= 1;
        obj->accel_y >>= 1;
        obj->state_3++;
        update_sequence(obj);
    }
}
