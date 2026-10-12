/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80044618_slot28[];

void ignore_object_empty_26_slot28(Object *obj) {
}

void call_state_1_handler_then_advance_sequence_3_slot28(Object *obj) {
    data_80044618_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
}
