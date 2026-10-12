/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void branch_script_if_other_health_below_arg(Object *object) {
    if ((s16) object->other->health < *(s16 *) data_80189460++) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}
