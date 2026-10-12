/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8007eb48_slot2b[];
extern ObjectFn data_8007ed98_slot2b[];
extern ObjectFn data_8007eda8_slot2b[];
extern Object *data_8007ef34_slot2b;
void set_sequence_from_table_index_entry_slot2b(Object *o);

void set_variant_and_sequence_from_table_slot2b(Object *o, u8 a) {
    o->variant_index = a;
    set_sequence_entry(o, data_8007eb48_slot2b[a]);
}

void call_handler_by_state_0_with_parent_saved_2_slot2b(Object *o) {
    data_8007ef34_slot2b = o->parent;
    data_8007ed98_slot2b[o->state_0](o);
}

void init_from_global_and_call_by_table_index_slot2b(Object *o) {
    Object *p;

    o->state_0++;
    p = data_8007ef34_slot2b;
    o->field_1c = p->field_1c;
    o->field_0c = p->field_0c;
    o->image_row = data_8007ef34_slot2b->image_row;
    o->facing_side = data_8007ef34_slot2b->facing_side;
    data_8007eda8_slot2b[o->table_index](o);
}

void set_spawn_flag_and_start_table_sequence_slot2b(Object *o) {
    o->spawn_flag = 1;
    o->field_0e = data_8007ef34_slot2b->field_0e;
    set_sequence_from_table_index_entry_slot2b(o);
}
