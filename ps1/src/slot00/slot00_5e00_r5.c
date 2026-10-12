/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_3_start_attack_param_sequence_slot00(Object *obj) {
    obj->state_3++;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    if (obj->attack_param_d != 0) {
        obj->state_3 = 2;
        obj->field_225 = 1;
        if (obj->counter == 0) {
            obj->field_225 = 0;
            add_to_meter_clamped(obj, 1);
        }
        play_side_sound_at(obj, obj->side, 9);
        set_sequence_by_side_and_mode(obj, 0x2b);
    } else {
        set_sequence_and_tables_from_direction(obj);
    }
}

void update_sequence_or_finish_on_flag_5_slot00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if (consume_active_frame_flag(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
