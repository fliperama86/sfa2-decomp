/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800469cc_slot28[];
extern void (*data_800469d0_slot28[])(Object *);
extern ObjectFn data_800469d8_slot28[];

void init_object_and_start_indexed_sequence_3_slot28(Object *o) {
    o->field_0e = 0;
    o->field_0c = 0;
    o->facing_side = 0;
    o->variant_index = 0;
    o->state_0++;
    set_sequence_from_table_5(o, data_800469cc_slot28[o->table_index], (SequenceStep **)o->box_tables);
}

void call_table_handler_by_table_index_4_slot28(Object *obj) {
    data_800469d0_slot28[obj->table_index](obj);
    obj->field_01 = 1;
}

void call_state_1_handler_then_advance_sequence_4_slot28(Object *obj) {
    data_800469d8_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
}

void advance_state_1_if_variant_set_4_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_1 += 1;
    }
}
