/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012b8dc(Object *object) {
    if (game_state.field_4d != 0) {
        set_win_or_lose_state_by_health(object);
    } else if (func_8012f56c(object)) {
        set_win_state_and_store_result_byte(object);
    } else if (object->mode_flag_2 != 0) {
        if (!func_80149d48(object)) {
            reset_object_to_state_1_0_0_2_sequence_1(object);
        } else {
            ref_other.p = object->other;
            if (ref_other.p->field_157 == 0) goto reset;
            if (ref_other.p->active_state == 0) goto copy;
            goto reset;
        }
    } else if (get_pressed_low_byte(object)) {
        set_state_1_0_5_and_read_input_direction(object);
    } else if ((u8)is_input_bit_1000_held(object)) {
        func_80130280(object);
    } else if (!func_8012f970(object)) {
        object->state_3++;
        object->facing_side = object->saved_facing;
        set_sequence_by_side(object, 0x1a);
    } else if ((u8)is_button_4000_held_or_other_flag(object)) {
        goto copy;
    } else {
reset:
        object->facing_side = object->saved_facing;
        object->state_3 = 0;
        object->field_157 = 0;
        set_sequence_by_side(object, 0x16);
    }
    return;
copy:
    object->facing_side = object->saved_facing;
    update_sequence(object);
}
