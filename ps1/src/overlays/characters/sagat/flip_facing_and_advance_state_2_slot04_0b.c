/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void flip_facing_and_advance_state_2_slot04_0b(Object *o) {
    o->state_2++;
    o->facing_side ^= 1;
    call_kind_9_handler_for_player(o);
    o->countdown = 2;
    advance_sequence_checked(o);
}
