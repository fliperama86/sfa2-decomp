/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void set_move_state_dir_0_countdown_if_below(Object *object) {
    set_limits_from_script_word(object);
    object->countdown_3 = *script_cursor++;
    script_work_value = script_work_value - object->compare_limit;
    if ((s16)script_work_value >= 0) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 5;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 2;
        object->state_3 = 0;
        object->variant_index = 0;
        object->direction_flag = 0;
    }
}

void set_move_state_dir_1_countdown_if_above(Object *object) {
    set_limits_from_script_word(object);
    object->countdown_3 = *script_cursor++;
    script_work_value = script_work_value - object->compare_limit;
    if ((s16)script_work_value < 0) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 6;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 2;
        object->state_3 = 0;
        object->variant_index = 1;
        object->direction_flag = 1;
    }
}

void set_move_state_dir_by_limit_countdown(Object *object) {
    u16 old;
    u16 value;
    set_limits_from_script_word(object);
    value = *script_cursor++;
    old = script_work_value;
    object->countdown_3 = value;
    script_work_value = old - object->compare_limit + 3;
    if (script_work_value < 6) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 7;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 2;
        object->state_3 = 0;
        object->variant_index = 0;
        object->direction_flag = 0;
        if (old >= object->compare_limit) {
            object->variant_index = 1;
            object->direction_flag = 1;
        }
    }
}
