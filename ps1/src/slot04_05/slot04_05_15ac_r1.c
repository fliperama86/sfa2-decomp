/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c1424_slot04_05[];

void start_state_3_motion_with_side_mirroring_slot04_05(Object *obj) {
    int a = 0x1c;

    obj->state_3++;
    obj->active_flag_4 = 1;
    add_to_meter_clamped(obj, 9);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->vel_x = data_801c1424_slot04_05[obj->direction_index * 2];
    obj->vel_y = data_801c1424_slot04_05[obj->direction_index * 2 + 1];
    obj->accel_x = data_801c1424_slot04_05[obj->direction_index * 2 + 2];
    obj->accel_y = data_801c1424_slot04_05[obj->direction_index * 2 + 3];
    if (obj->facing_side == 0) {
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    if (obj->field_49 != 0) {
        a = 0x38;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}

void update_sequence_advance_state_3_on_flag_slot04_05(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->state_3++;
    }
}
