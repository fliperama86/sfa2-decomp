/* Reconstruction. Names/roles inferred, not original symbols.
 * Unresolved: the 20e/20f split in read_script_word_and_dispatch/9f4/a7c/b1c. The original does
 * sll 16 / sra 24 (and a move) where this compiles to srl 8. Calls without an
 * argument (nop in the delay slot) are written as unprototyped declarations. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void read_or_enter_script_by_random_bit(Object *object) {
    if (test_random_bit_in_table(object, (u32 *)table_8017d450) != 0) {
        read_script_word_or_restore_script(object);
    } else {
        enter_script_mode_1_and_read_opcode(object);
    }
}

void clear_or_restore_script_by_mode_word(Object *object) {
    if (object->field_20c == 0) {
        clear_three_script_bytes(object);
    } else if (object->field_20c == 2) {
        reset_script_to_saved_word(object);
    } else if (object->field_20c == 4) {
        if (test_random_bit_in_table(object, (u32 *)table_8017d450) != 0) {
            clear_three_script_bytes(object);
        } else {
            reset_script_to_saved_word(object);
        }
    }
}

void reset_script_to_saved_word(Object *object) {
    object->script_mode = 0;
    object->field_224 = 0;
    object->field_25d = 0;
    object->field_20c = object->saved_script_word;
    restore_script_pointer_and_read_word(object);
}

void func_8014ccb0(Object *object) {
    table_8017d28c[object->field_208](object);
}

void func_8014ccf0(Object *object) {
    table_8017d2a8[object->field_209](object);
}

void check_script_wait_countdown_3(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object) != 0) {
        init_object_mode_2(object);
    } else if (object->countdown_3 == 0 || --object->countdown_3 == 0) {
        clear_three_script_bytes(object);
    }
}

void init_mode_2_or_clear_bytes_if_flag_set(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object) != 0) {
        init_object_mode_2(object);
    } else if (object->state_2 == 0 || object->field_157 != 0) {
        clear_three_script_bytes(object);
    }
}

void init_mode_2_or_clear_bytes_if_flag_clear(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object) != 0) {
        init_object_mode_2(object);
    } else if (object->state_2 == 0 || object->field_157 == 0) {
        clear_three_script_bytes(object);
    }
}

void check_script_wait_other_active_state(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object) != 0) {
        init_object_mode_2(object);
    } else if (object->other->active_state != 1) {
        clear_three_script_bytes(object);
    }
}

void check_script_wait_other_height_limit(Object *object) {
    Object *other;
    if ((u8)roll_random_bit_check_from_kind_table(object) != 0) {
        init_object_mode_2(object);
    } else {
        other = object->other;
        if (other->active_state != 1
            || ((s16)other->pos_y >= (s16)(other->ground_y - (object->limit_lo + (object->limit_hi << 8)))
                && other->vel_y > 0)) {
            clear_three_script_bytes(object);
        }
    }
}

void check_script_cond_countdown_and_set_state(Object *object) {
    if ((u8)roll_random_bit_check_from_kind_table(object) != 0) {
        init_object_mode_2(object);
    } else if (object->countdown_3 == 0 || --object->countdown_3 == 0) {
        clear_three_script_bytes(object);
    } else if (find_active_wide_box_and_check_other(object) != 0) {
        object->field_208 = 3;
        object->field_209 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 5;
        object->state_3 = 0;
        object->active_flag_3 = 1;
        object->active_flag_5 = 0;
        object->facing_side = object->saved_facing;
    }
}
