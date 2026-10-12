/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void call_handler_by_mode_flag_2(Object *object) {
    if (object->field_60 == 1 && object->field_6b != 0 && object->frame->handler_index != 0) {
        if (object->mode_flag_2 != 0) {
            set_sequence_by_action_timer_and_other(object);
        } else {
            update_sequence_or_select_by_input_bits(object);
        }
    }
}

void update_sequence_or_select_by_input_bits(Object *object) {
    if ((object->input_bits_2 & 0x2000) == 0) {
        update_sequence(object);
    } else {
        object->field_157 = 0;
        if (object->action_timer != 0) {
            set_sequence_c_or_e_by_pressed_bits(object);
        } else {
            if ((object->input_bits_2 & 0x4000) == 0) {
                set_sequence_by_side_and_mode_2(object, 0xc);
            } else {
                object->field_157 = 1;
                set_sequence_by_side_and_mode_2(object, 0xe);
            }
        }
    }
}
