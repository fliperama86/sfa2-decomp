/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014bcd8(Object *object) {
    if (func_80149d48(object)) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void branch_script_if_other_active_variant_1(Object *object) {
    Object *other = object->other;
    if (other->active_state == 1 && other->variant_index == 1) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void branch_script_if_other_active_variant_0(Object *object) {
    Object *other = object->other;
    if (other->active_state == 1 && other->variant_index == 0) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void branch_script_if_other_active_variant_ff(Object *object) {
    Object *other = object->other;
    if (other->active_state == 1 && other->variant_index == 0xff) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void branch_script_if_other_check_true(Object *object) {
    if ((u8)func_8014e758(object)) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}
