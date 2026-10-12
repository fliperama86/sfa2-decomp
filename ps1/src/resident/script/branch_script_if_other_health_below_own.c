/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void branch_script_if_other_health_below_own(Object *object) {
    if ((s16)object->other->health < (s16)object->health) {
        read_script_word_and_dispatch(object);
    } else {
        skip_script_block_then_read_opcode(object);
    }
}

void branch_script_if_other_active_moving_up(Object *object) {
    Object *other = object->other;
    if (other->active_state != 1 || other->vel_y >= 0) {
        skip_script_block_then_read_opcode(object);
    } else {
        read_script_word_and_dispatch(object);
    }
}

void branch_script_unless_other_moving_up(Object *object) {
    Object *other = object->other;
    if (other->active_state == 1 && other->vel_y < 0) {
        skip_script_block_then_read_opcode(object);
    } else {
        read_script_word_and_dispatch(object);
    }
}
