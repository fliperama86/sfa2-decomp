/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_8007ee08_slot2b[];

void advance_sequence_checked_wrapper_3_slot2b(Object *o) {
    advance_sequence_checked(o);
}

void call_handler_by_state_1_6_slot2b(Object *o) {
    data_8007ee08_slot2b[o->state_1](o);
}
