/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void set_countdown_state_8_or_skip(Object *object) {
    if ((*(unsigned *)&object->field_208 & 0xffff0000) == 0) {
        branch_script_if_countdown_3_zero(object);
    } else {
        set_countdown_from_table_with_random(object);
        if (object->countdown_3 == 0) {
            read_script_word_and_dispatch(object);
        } else {
            object->field_208 = 0;
            object->field_209 = 8;
            set_state_one_unless_set(object);
        }
    }
}

void set_state_one_if_linked_object_3(Object *object) {
    if (object->link_flags == 0 || object->linked_object_3 == 0) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 9;
        set_state_one_unless_set(object);
    }
}

void set_state_one_unless_other_check(Object *object) {
    if ((u8)func_8014e758(object)) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 10;
        set_state_one_unless_set(object);
    }
}

void branch_script_if_countdown_3_zero(Object *object) {
    set_countdown_from_table_with_random(object);
    if (object->countdown_3 == 0) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 0;
        set_state_one_unless_set(object);
    }
}

void set_countdown_one_mode_eight_state_one(Object *object) {
    object->countdown_3 = 1;
    object->field_208 = 0;
    object->field_209 = 8;
    set_state_one_unless_set(object);
}

void set_countdown_from_script_state_one(Object *object) {
    u16 value = *data_80189460++;
    object->field_208 = 0;
    object->field_209 = 8;
    object->countdown_3 = value;
    set_state_one_unless_set(object);
}

void set_state_if_script_value_below_limit(Object *object) {
    set_limits_from_script_word(object);
    object->countdown_3 = *data_80189460++;
    data_80189464 = data_80189464 - object->compare_limit;
    if ((s16)data_80189464 >= 0) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_209 = 0xb;
        object->field_208 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}

void set_state_if_script_value_at_least_limit(Object *object) {
    set_limits_from_script_word(object);
    object->countdown_3 = *data_80189460++;
    data_80189464 = data_80189464 - object->compare_limit;
    if ((s16)data_80189464 < 0) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_209 = 0xc;
        object->field_208 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}

void set_state_one_unless_other_check_d(Object *object) {
    if ((u8)func_8014e718(object)) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 0xd;
        set_state_one_unless_set(object);
    }
}

void func_8014adb8(Object *object) {
    if (select_direction_from_distance_table(object)) {
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

void clear_active_flag_mode_five_state_one(Object *object) {
    object->active_flag = 0;
    object->field_208 = 0;
    object->field_209 = 5;
    set_state_one_unless_set(object);
}

void set_state_one_unless_set(Object *object) {
    if ((*(unsigned *)&object->state_0 & 0xffffff) != 1) {
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}

void func_8014ae88(Object *object) {
    table_8017d010[object->script_arg >> 1](object);
}

void set_move_state_dir_0_if_below_limit(Object *object) {
    set_limits_from_script_word(object);
    data_80189464 = data_80189464 - object->compare_limit;
    if ((s16)data_80189464 >= 0) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_208 = 1;
        object->state_0 = 1;
        object->field_209 = 0;
        object->state_1 = 0;
        object->state_2 = 2;
        object->state_3 = 0;
        object->variant_index = 0;
        object->direction_flag = 0;
    }
}

void set_move_state_dir_1_if_below_limit(Object *object) {
    set_limits_from_script_word(object);
    data_80189464 = data_80189464 - object->compare_limit;
    if ((s16)data_80189464 >= 0) {
        read_script_word_and_dispatch(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 2;
        object->state_3 = 0;
        object->variant_index = 1;
        object->direction_flag = 1;
    }
}
