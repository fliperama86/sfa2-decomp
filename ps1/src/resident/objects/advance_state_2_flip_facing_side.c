/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void advance_state_2_flip_facing_side(Object *object) {
    object->sequence_changed_flag = 1;
    object->state_2++;
    object->facing_side ^= 1;
    call_kind_9_handler_for_player(object);
    object->countdown = (u8)object->countdown | 0x200;
    advance_sequence_checked(object);
}
