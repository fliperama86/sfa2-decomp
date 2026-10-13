/* Reconstruction. Names/roles inferred, not original symbols. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: script_branch_if_value_ge_compare_limit, script_branch_if_value_le_compare_limit (and r2 branch_script_if_value_ge_table_column, branch_script_if_table_column_ge_value): the original computes
   field_212 as sll 16 / sra 24 of the raw halfword; every form tried (>> 8, casts to s16/s8, int/unsigned/u16 locals,
   s16 pointer) folds to srl 8. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void init_script_state_by_script_arg(Object *object) {
    if (object->script_arg == 0) {
        u8 v;
        object->field_208 = 3;
        object->field_209 = 0;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 5;
        object->state_3 = 0;
        object->active_flag_3 = 1;
        object->active_flag_5 = 0;
        v = object->saved_facing;
        object->active_flag = 0;
        object->facing_side = v;
        set_limits_from_script_word(object);
        decode_input_word_into_object_fields(object);
    } else if (object->script_arg == 2) {
        u8 v;
        object->field_208 = 3;
        object->field_209 = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 5;
        object->state_3 = 0;
        object->active_flag_3 = 1;
        object->active_flag_5 = 1;
        v = object->saved_facing;
        object->active_flag = 0;
        object->facing_side = v;
        set_limits_from_script_word(object);
        decode_input_word_into_object_fields(object);
    }
}

void set_script_state_7_if_check_passes(Object *object) {
    if (object->script_arg == 0) {
        set_limits_from_script_word(object);
        decode_input_word_into_object_fields(object);
        if (func_8014dcc0(object)) {
            if (*(u32 *)&object->state_0 == 0x20101) {
                object->effect_request = 2;
            }
            object->field_208 = 4;
            object->field_209 = 0;
            object->state_0 = 1;
            object->state_1 = 0;
            object->state_2 = 7;
            object->state_3 = 0;
            object->active_flag_3 = 1;
            object->active_flag = 0;
            object->field_157 = 0;
            object->facing_side = object->saved_facing;
        } else {
            read_script_word_and_dispatch(object);
        }
    }
}

void set_script_state_8_if_check_passes(Object *object) {
    if (object->script_arg == 0) {
        set_limits_from_script_word(object);
        decode_input_word_into_object_fields(object);
        if (func_8014dcc0(object)) {
            if (*(u32 *)&object->state_0 == 0x20101) {
                object->effect_request = 2;
            }
            object->field_208 = 5;
            object->field_209 = 0;
            object->state_0 = 1;
            object->state_1 = 0;
            object->state_2 = 8;
            object->state_3 = 0;
            object->active_flag_3 = 1;
            object->active_flag = 0;
            object->field_157 = 0;
            object->facing_side = object->saved_facing;
        } else {
            read_script_word_and_dispatch(object);
        }
    }
}

void dispatch_script_op_by_script_arg(Object *object) {
    if (object->script_arg == 0) {
        object->field_224++;
        set_limits_from_script_word(object);
        table_8017d030[(s16)script_work_value >> 1](object);
    } else if (object->script_arg == 2) {
        skip_script_block_then_read_opcode(object);
    } else if (object->script_arg == 4) {
        object->field_224--;
        read_script_word_and_dispatch(object);
    }
}
