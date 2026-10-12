/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void inc_state_3_side_sequence_fifty_slot04_0b(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    set_sequence_by_side(obj, 0x32);
}
