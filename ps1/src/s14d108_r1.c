/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void count_down_then_clear_three_bytes(Object *object) {
    if (object->countdown_3 == 0 || --object->countdown_3 == 0) {
        clear_three_script_bytes(object);
    }
}

void check_script_cond_no_link(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object)) {
        init_object_mode_2(object);
    } else if (object->link_flags == 0 || object->linked_object_3 == 0) {
        clear_three_script_bytes(object);
    }
}

void check_script_cond_other_check(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object)) {
        init_object_mode_2(object);
    } else if ((u8)func_8014e758(object)) {
        clear_three_script_bytes(object);
    }
}

void check_script_wait_countdown_or_limit_ge(Object *object) {
    int hi, lo;
    if ((u8)roll_random_bit_check_from_kind_table(object)) {
        init_object_mode_2(object);
    } else if (object->countdown_3 == 0 || --object->countdown_3 == 0
        || (hi = object->limit_hi, lo = object->limit_lo, !((s16)((hi << 8) + lo) < (s16)object->compare_limit))) {
        clear_three_script_bytes(object);
    }
}

void check_script_wait_countdown_or_limit_lt(Object *object) {
    int hi, lo;
    if ((u8)roll_random_bit_check_from_kind_table(object)) {
        init_object_mode_2(object);
    } else if (object->countdown_3 == 0 || --object->countdown_3 == 0
        || (hi = object->limit_hi, lo = object->limit_lo, (s16)((hi << 8) + lo) < (s16)object->compare_limit)) {
        clear_three_script_bytes(object);
    }
}

void check_script_cond_other_check_2(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object)) {
        init_object_mode_2(object);
    } else if ((u8)func_8014e718(object)) {
        clear_three_script_bytes(object);
    }
}

void func_8014d3a8(Object *object) {
    data_8017d2e0[object->field_209](object);
}

void check_script_wait_limit_ge_or_push(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object)) {
        init_object_mode_2(object);
    } else if (object->state_2 == 0
        || (u16)(object->limit_lo | (object->limit_hi << 8)) >= object->compare_limit
        || (object->variant_index != 0 && object->bound_push_flag != 0)) {
        clear_three_script_bytes(object);
    }
}

void check_script_wait_limit_lt_or_push(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object)) {
        init_object_mode_2(object);
    } else if (object->state_2 == 0
        || (u16)(object->limit_lo | (object->limit_hi << 8)) < object->compare_limit
        || (object->variant_index != 0 && object->bound_push_flag != 0)) {
        clear_three_script_bytes(object);
    }
}
