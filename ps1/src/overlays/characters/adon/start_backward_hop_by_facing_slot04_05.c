/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_backward_hop_by_facing_slot04_05(Object *obj) {
    obj->state_3++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    spawn_three_child_objects_at_object_spawn(obj);
    if (obj->facing_side == 0) {
        obj->vel_x = 0xfffb0000;
    } else {
        obj->vel_x = 0x50000;
    }
    obj->accel_x = 0;
    obj->vel_y = 0x88000;
    obj->accel_y = -0x9000;
    set_sequence_by_side_and_mode(obj, 0x2b);
}
