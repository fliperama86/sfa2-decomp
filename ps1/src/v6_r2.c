/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void update_table_step_with_input_branches(Object *object) {
    int index;
    if (apply_table_step_to_pos_x(object) != 0) {
        object->state_3 = object->state_3 + 1;
        set_side_flag(object);
        object->once_flag_2 = 0;
        object->done_flag = 0;
        object->flag_25a_set = 0;
    } else {
        store_image_row_for_hit_type(object);
    }
    if ((s16)object->health < 0) {
        if ((s16)object->countdown >= 8) {
            object->state_3 = 3;
            set_sequence_by_side_and_mode_2(object, 0x1b);
        }
    } else if (object->hit_type == 8 && object->mode_flag_2 != 0) {
        call_meter_action_or_default_sequence(object);
    } else if (object->hit_type != 8 || (object->input_bits_2 & 0x2000) == 0) {
        update_sequence(object);
    } else {
        index = 0xc;
        object->field_60 = 1;
        object->field_157 = 0;
        if ((object->input_bits_2 & 0x4000) != 0) {
            object->field_157 = 1;
            index = 0xe;
        }
        set_sequence_by_side_and_mode_2(object, index);
    }
}
