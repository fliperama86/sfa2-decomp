/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void set_leftward_velocity_on_step_flag_byte(Object *object) {
    if ((u8)object->step_flags != 0) {
        object->countdown = 0xf;
        object->vel_x = 0xfff40000;
        object->accel_x = 0xfff80000;
        object->state_2 = object->state_2 + 1;
        object->step_flags = object->step_flags & 0xff00;
    }
}
