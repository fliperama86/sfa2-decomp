/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012b628(Object *object) {
    int t;
    if (game_state.field_4d != 0) {
        set_win_or_lose_state_by_health(object);
        return;
    }
    if (func_8012f56c(object)) {
        set_win_state_and_store_result_byte(object);
        return;
    }
    if (object->mode_flag_2 != 0) {
        if (!func_80149d48(object)) {
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
            func_80130280(object);
            return;
        }
        if (!func_8012f970(object)) {
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
