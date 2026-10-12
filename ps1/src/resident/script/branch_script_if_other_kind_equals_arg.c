/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void branch_script_if_other_kind_equals_arg(Object *object) {
    s16 v = *data_80189460++;
    if (object->other->kind == v) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void branch_script_if_kind_equals_arg(Object *object) {
    s16 v = *data_80189460++;
    if (object->kind == v) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}
