/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801c7c98_slot04_09[];

void enter_state_add_meter_start_sequence_by_flag_slot04_09(Object *obj) {
    int n = 0x1f;

    obj->step_count = 0;
    obj->state_3++;
    if (obj->field_49 != 0) {
        obj->state_3 = 9;
        obj->active_flag_4 = 1;
        obj->field_225 = 1;
    }
    if (obj->counter == 0) {
        add_to_meter_clamped(obj, 5);
    }
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->field_49 != 0) {
        n = 0x46;
    }
    set_sequence_by_side_and_mode(obj, n);
}

void launch_jump_velocity_from_table_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->active_state = 1;
        obj->accel_x = -0x8000;
        obj->accel_y = -0x6000;
        obj->state_3++;
        obj->vel_x = data_801c7c98_slot04_09[obj->direction_index];
        obj->vel_y = data_801c7c98_slot04_09[obj->direction_index + 1];
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
        set_sequence_by_side_and_mode(obj, 0x20);
    } else {
        update_sequence(obj);
    }
}
