/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void advance_state_and_sequence_twice_flicker_slot28(Object *o) {
    o->field_01 = 0;
    if (((Slot28Obj *)o)->field_3a < 0) {
        o->state_0++;
    }
    advance_sequence_checked(o);
    advance_sequence_checked(o);
    o->variant_index = o->variant_index ^ 1;
    if (o->variant_index != 0) {
        o->field_01 = 1;
    }
}
