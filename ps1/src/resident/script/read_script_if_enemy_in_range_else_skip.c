/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void read_script_if_enemy_in_range_else_skip(Object *object) {
    if (check_enemy_unit_or_other_in_range(object)) {
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
    if ((u8)is_other_state_one_with_frame_boxes(object)) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}
