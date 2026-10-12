/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_table_step_then_choose_sequence_c_or_e(Object *object) {
    int index;
    if (apply_table_step_to_pos_x(object) != 0) {
        object->state_3 = object->state_3 + 1;
        object->facing_side = (object->facing_flag ^ 1) & 1;
        set_state_1_1_2_0_or_enter_state_1_6_2(object);
    } else if ((s16)object->health < 0) {
        set_sequence_c_or_e_by_pressed_bits(object);
    } else if (object->mode_flag_2 != 0) {
        call_meter_action_or_default_sequence(object);
    } else {
        index = 0xc;
        if ((object->input_bits_2 & 0x2000) == 0) {
            update_sequence(object);
        } else {
            object->field_157 = 0;
            if (object->action_timer != 0) {
                set_sequence_c_or_e_by_pressed_bits(object);
            } else {
                if ((object->input_bits_2 & 0x4000) != 0) {
                    object->field_157 = 1;
                    index = 0xe;
                }
                set_sequence_by_side_and_mode_2(object, index);
            }
        }
    }
}
