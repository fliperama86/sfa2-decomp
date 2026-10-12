/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801bfe08_slot04_00[];

void launch_object_with_direction_velocity_slot04_00(Object *obj) {
    int a = 0x1b;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 9);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->accel_x = -0x8000;
    obj->accel_y = -0x6000;
    obj->vel_x = data_801bfe08_slot04_00[obj->direction_index];
    obj->vel_y = data_801bfe08_slot04_00[obj->direction_index + 1];
    if (obj->field_49 != 0) {
        a = 0x3b;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}
