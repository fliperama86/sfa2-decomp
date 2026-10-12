/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void check_script_wait_countdown_or_push(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object)) {
        init_object_mode_2(object);
    } else if (object->state_2 == 0 || object->countdown_3 == 0
        || (object->countdown_3--, object->variant_index != 0 && object->bound_push_flag != 0)) {
        clear_three_script_bytes(object);
    }
}

void func_8014d66c(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object)) {
        init_object_mode_2(object);
    } else if (object->state_2 == 0 || object->countdown_3 == 0
        || (object->countdown_3--, (u16)(object->limit_lo | (object->limit_hi << 8)) >= object->compare_limit)
        || (object->variant_index != 0 && object->bound_push_flag != 0)) {
        clear_three_script_bytes(object);
    }
}

void func_8014d720(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object)) {
        init_object_mode_2(object);
    } else if (object->state_2 == 0 || object->countdown_3 == 0
        || (object->countdown_3--, (u16)(object->limit_lo | (object->limit_hi << 8)) < object->compare_limit)
        || (object->variant_index != 0 && object->bound_push_flag != 0)) {
        clear_three_script_bytes(object);
    }
}
