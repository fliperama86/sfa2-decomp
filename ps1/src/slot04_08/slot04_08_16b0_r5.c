/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c3c4c_slot04_08[];

void advance_state_3_set_sequence_from_direction_slot04_08(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 5);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->field_49 != 0) {
        obj->field_225 = 1;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x39);
}

void update_sequence_or_start_range_hit_sequence_slot04_08(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else if (t & 0xff) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x30, *(s16 *)(data_801c3c4c_slot04_08 + (obj->direction_index & 0xfe))) != 0) {
            obj->state_3++;
            play_sound_at(obj, obj->side ^ 1, 0x31a);
            play_side_sound_at(obj, obj->side, 0x12);
            add_to_meter_clamped(obj, 0x20);
            set_sequence_by_side_and_mode(obj, 0x1c);
        } else {
            update_sequence(obj);
        }
    } else {
        update_sequence(obj);
    }
}
