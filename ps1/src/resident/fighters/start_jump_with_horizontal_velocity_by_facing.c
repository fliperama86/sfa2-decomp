/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void start_jump_with_horizontal_velocity_by_facing(Object *object) {
    int v;
    object->variant_index = 0xff;
    object->active_state = 0xff;
    object->vel_y = 0x10000;
    object->accel_y = -0x6800;
    object->countdown = 0;
    object->accel_x = 0;
    object->state_3 = object->state_3 + 1;
    if (object->facing_flag == 0) {
        v = -0x60000;
    } else {
        v = 0x60000;
    }
    object->vel_x = v;
}
