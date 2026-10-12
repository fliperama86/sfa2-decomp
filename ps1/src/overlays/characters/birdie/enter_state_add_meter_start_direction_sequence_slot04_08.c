/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s16 data_801c3d9c_slot04_08[];
extern u16 data_801c3d8c_slot04_08[];

void enter_state_add_meter_start_direction_sequence_slot04_08(Object *obj) {
    obj->state_3++;
    add_to_meter_clamped(obj, 5);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->active_flag_4 = 1;
    if (obj->field_49 != 0) {
        obj->field_225 = 1;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x39);
}

void update_state_3_range_check_start_hit_slot04_08(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else if ((t & 0xff) && (u8)try_force_other_state_if_box_in_range(obj, -0x30, data_801c3d9c_slot04_08[(obj->direction_index >> 1) * 2]) != 0) {
        obj->state_3++;
        obj->countdown = data_801c3d8c_slot04_08[(obj->direction_index >> 1) * 2];
        add_to_meter_clamped(obj, 0x15);
        play_sound_at(obj, obj->side ^ 1, 0x31a);
        play_side_sound_at(obj, obj->side, 0x12);
        set_sequence_by_side_and_mode(obj, 0x5d);
    } else {
        update_sequence(obj);
    }
}
