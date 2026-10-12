/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c1478_slot04_05[];

void start_state_3_motion_from_velocity_table_slot04_05(Object *obj) {
    int a = 0x22;

    obj->state_3++;
    obj->active_flag_4 = 1;
    add_to_meter_clamped(obj, 7);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->vel_x = data_801c1478_slot04_05[obj->direction_index * 2];
    obj->vel_y = data_801c1478_slot04_05[obj->direction_index * 2 + 1];
    obj->accel_x = data_801c1478_slot04_05[obj->direction_index * 2 + 2];
    obj->accel_y = data_801c1478_slot04_05[obj->direction_index * 2 + 3];
    if (obj->field_49 != 0) {
        a = 0x35;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}

void update_sequence_play_sound_when_flag_zero_slot04_05(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 0) {
        obj->active_state = 1;
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0x10);
    }
}
