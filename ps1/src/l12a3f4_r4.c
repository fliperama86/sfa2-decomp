/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void select_state_by_input_or_update_sequence(Object *object) {
    u16 saved;
    if (object->mode_flag_2 != 0) {
        update_sequence_or_return_to_idle(object);
    } else if (get_pressed_low_byte(object)) {
        set_state_1_0_5_and_read_input_direction(object);
    } else if ((u8)is_input_bit_1000_held(object)) {
        enter_state_1_3_with_input_variant_and_sequence(object);
    } else if ((u8)is_single_horizontal_input_held(object)) {
        start_move_state_1_0_2_1_with_sequence_by_input(object);
    } else if ((u8)is_button_4000_held_or_other_flag(object)) {
        object->state_3 = object->state_3 + 1;
        saved = object->step_timer;
        set_sequence_by_side(object, (u8)object->step_flags + 0xd);
        object->step_timer = saved;
    } else if (find_nearby_enemy_and_get_flag(object)) {
        enter_state_2_6_by_button_input(object);
    } else if ((s16)object->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(object);
    } else {
        object->facing_side = object->saved_facing;
        update_sequence(object);
    }
}
