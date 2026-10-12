/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8003743c_slot28[];

void init_object_and_start_indexed_sequence_slot28(Object *o) {
    o->field_0e = 0;
    o->field_0c = 0;
    o->facing_side = 0;
    o->variant_index = 0;
    o->state_0++;
    set_sequence_from_table_5(o, data_8003743c_slot28[o->table_index], (SequenceStep **)o->box_tables);
}
