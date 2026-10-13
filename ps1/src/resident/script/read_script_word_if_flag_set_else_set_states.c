/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void read_script_word_if_flag_set_else_set_states(Object *object) {
    if (object->field_157 != 0) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 1;
        object->state_3 = 0;
    }
}

void read_script_word_if_flag_clear_else_set_states(Object *object) {
    if (object->field_157 == 0) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 2;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 1;
        object->state_3 = 2;
    }
}

void branch_script_if_other_not_active_1(Object *object) {
    if (object->other->active_state != 1) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_209 = 3;
        object->field_208 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}

void branch_script_by_other_height_and_vel(Object *object) {
    Object *other = object->other;
    set_limits_from_script_word(object);
    if (other->active_state != 1) {
        read_script_word_and_dispatch(object);
    } else {
        script_work_value = other->ground_y - script_work_value;
        if ((s16)script_work_value < other->pos_y && other->vel_y > 0) {
            read_script_word_and_dispatch(object);
        } else {
            object->field_209 = 4;
            object->field_208 = 0;
            object->state_0 = 1;
            object->state_1 = 0;
            object->state_2 = 0;
            object->state_3 = 0;
        }
    }
}

void set_states_if_wide_box_check_else_read_script(Object *object) {
    if (find_active_wide_box_and_check_other(object)) {
        object->field_208 = 3;
        object->field_209 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 5;
        object->state_3 = 0;
        object->active_flag_3 = 1;
        object->active_flag_5 = 0;
        object->facing_side = object->saved_facing;
    } else {
        read_script_word_and_dispatch(object);
    }
}

void init_script_state_with_countdown(Object *object) {
    object->field_208 = 0;
    object->field_209 = 6;
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 0;
    object->state_3 = 0;
    object->countdown_3 = *script_cursor++;
    if (find_active_wide_box_and_check_other(object)) {
        object->field_208 = 3;
        object->field_209 = 0;
        object->state_2 = 5;
        object->active_flag_3 = 1;
        object->active_flag_5 = 0;
        object->facing_side = object->saved_facing;
    }
}
