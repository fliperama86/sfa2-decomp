/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void branch_script_if_no_link(Object *object) {
    if (object->link_flags == 0 && object->linked_object_3 == 0) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}
