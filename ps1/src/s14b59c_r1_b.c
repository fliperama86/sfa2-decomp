/* Reconstruction. Names/roles inferred, not original symbols. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: script_branch_if_value_ge_compare_limit, script_branch_if_value_le_compare_limit (and r2 branch_script_if_value_ge_table_column, branch_script_if_table_column_ge_value): the original computes
   field_212 as sll 16 / sra 24 of the raw halfword; every form tried (>> 8, casts to s16/s8, int/unsigned/u16 locals,
   s16 pointer) folds to srl 8. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void script_branch_on_random_bit(Object *object) {
    if (!(get_random_byte() & 1)) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void script_branch_if_meter_ge_value(Object *object) {
    s16 *p = (s16 *)data_80189460;
    data_80189460 = (u16 *)(p + 1);
    if ((s16)object->meter >= *p) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void script_branch_if_other_active_state_1(Object *object) {
    if (object->other->active_state == 1) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void func_8014ba30(Object *object) {
    if (object->other->field_157 != 0) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void func_8014ba78(Object *object) {
    if (object->other->field_15b != 0) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void script_branch_if_other_linked_state_1(Object *object) {
    Object *other = object->other;
    if (other->link_flags == 0 || other->linked_object_3 == 0 || ((Object *)other->linked_object_3)->state_0 != 1) {
        skip_script_block_then_read_opcode(object);
    } else {
        read_script_word_and_dispatch(object);
    }
}
