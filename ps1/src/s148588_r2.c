/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void call_state_1_handler_4(Object *object) {
    table_8017cd38[object->state_1](object);
}

void start_sequence_when_parent_active(Object *object) {
    ref_other.p = object->parent;
    if (ref_other.p->active_state != 0) {
        object->state_1 = object->state_1 + 1;
        set_sequence_from_table_5(object, 15, seqs_8017c7f8);
    }
}
