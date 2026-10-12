/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void update_state_3_step_with_input_branches_a(Object *object) {
    int t;
    if (game_state.field_4d != 0) {
        set_win_or_lose_state_by_health(object);
        return;
    }
    if (is_other_object_state_0_and_1_six_one(object)) {
        set_win_state_and_store_result_byte(object);
        return;
    }
    if (object->mode_flag_2 != 0) {
        if (!check_enemy_unit_or_other_in_range(object)) {
            reset_object_to_state_1_0_sequence_0(object);
            return;
        }
        ref_other.p = object->other;
        if (ref_other.p->active_state != 0) {
            goto skip;
        }
        t = ref_other.p->field_157;
        goto test;
    } else {
        if (get_pressed_low_byte(object)) {
            set_state_1_0_5_and_read_input_direction(object);
            return;
        }
        if ((u8)is_input_bit_1000_held(object)) {
            enter_state_1_3_with_input_variant_and_sequence(object);
            return;
        }
        if (!find_nearby_enemy_and_get_flag(object)) {
            object->state_3 = object->state_3 + 1;
            object->facing_side = object->saved_facing;
            set_sequence_by_side(object, 0x17);
            return;
        }
        t = (u8)is_button_4000_pressed(object);
test:
        if (t) {
            object->state_3 = 2;
            object->field_157 = 1;
            object->facing_side = object->saved_facing;
            set_sequence_by_side(object, 0x19);
            return;
        }
    }
skip:
    object->facing_side = object->saved_facing;
    update_sequence(object);
}
