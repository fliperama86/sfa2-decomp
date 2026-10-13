/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void skip_script_block_then_read_opcode(Object *object) {
    data_8018946c = object->field_224;
    while (call_script_op_by_high_bits(object) != 0) {
    }
    read_script_word_and_dispatch(object);
}

void enter_script_mode_by_arg_or_random_bit(Object *object) {
    if (object->script_arg == 0) {
        enter_script_mode_save_state(object);
    } else if (object->script_arg == 2) {
        if (test_random_bit_in_table(object, (u32 *)table_8017d798) != 0) {
            enter_script_mode_save_state(object);
        } else {
            read_script_word_and_dispatch(object);
        }
    }
}

void enter_script_mode_save_state(Object *object) {
    object->script_mode = 1;
    object->saved_script_word = object->field_20c;
    object->script_pointer = (s32)script_cursor;
    start_random_script_from_table_index(object);
    read_script_word_and_dispatch(object);
}
