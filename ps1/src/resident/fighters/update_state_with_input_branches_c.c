/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void update_state_with_input_branches_c(Object *object) {
    if (object->mode_flag_2 != 0) {
        if ((s16)object->step_flags < 0) {
            reset_object_to_state_1_0_0_2_sequence_1(object);
        } else {
            object->facing_side = object->saved_facing;
            update_sequence(object);
        }
    } else if (get_pressed_low_byte(object)) {
        set_state_1_0_5_and_read_input_direction(object);
    } else if ((u8)is_input_bit_1000_held(object)) {
        enter_state_1_3_with_input_variant_and_sequence(object);
    } else if (find_nearby_enemy_and_get_flag(object)) {
        object->state_3 = 2;
        object->field_157 = 1;
        object->facing_side = object->saved_facing;
        set_sequence_by_side(object, 0x19);
    } else if ((u8)is_only_horizontal_input_held(object)) {
        start_move_state_1_0_2_1_with_sequence_by_input(object);
    } else if ((s16)object->step_flags < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(object);
    } else {
        object->facing_side = object->saved_facing;
        update_sequence(object);
    }
}
