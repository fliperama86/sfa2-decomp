/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectFn data_8002c7b0_slot28[];

void call_handler_by_state_1_show_and_advance_slot28(Object *obj) {
    data_8002c7b0_slot28[obj->state_1](obj);
    obj->field_01 = 1;
    advance_sequence_checked(obj);
}

void set_sequence_0f_when_variant_set_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_1++;
        set_sequence_from_table_5(obj, 0xf, (SequenceStep **)obj->box_tables);
    }
}
