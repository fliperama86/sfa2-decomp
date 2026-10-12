/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void launch_arc_and_start_sequence_at_sequence_end_slot04_09(Object *obj) {
    if (!((s16)obj->step_flags & 0x8000)) {
        update_sequence(obj);
    } else {
        obj->active_state = 1;
        obj->vel_x = 0x80000;
        obj->vel_y = 0x80000;
        obj->accel_x = -0x8000;
        obj->accel_y = -0x6000;
        obj->state_3++;
        if (obj->facing_side == 0) {
            obj->vel_x = 0xfff80000;
            obj->accel_x = 0x8000;
        }
        set_sequence_by_side_and_mode(obj, 0x47);
    }
}
