/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 is_pos_x_beyond_box_margin_slot04_02(Object *obj);

void start_sidestep_set_facing_from_input_slot04_02(Object *obj) {
    Object *o = obj->other;
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(o, o->side, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if (is_pos_x_beyond_box_margin_slot04_02(obj) != 0) {
            obj->facing_side = obj->facing_side + 1;
        }
    } else if (obj->input_bits & 0x8000) {
        obj->facing_side = 1;
    }
    set_sequence_by_side_and_mode(obj, (obj->input_dir_second >> 1) + 0x18);
}

void hit_other_on_flag_then_update_sequence_slot04_02(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 5, 0xf, 0, 1, 0);
    }
    update_sequence(obj);
}

void update_sequence_or_flip_facing_and_reset_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->facing_side = obj->facing_side ^ 1;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
