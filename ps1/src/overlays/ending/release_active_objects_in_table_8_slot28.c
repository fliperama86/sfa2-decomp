/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051a98_slot28[];
extern u16 data_8003f184_slot28[];
extern u16 data_8003f584_slot28[];
extern void (*data_8003f9c0_slot28[])(Object *);
extern u8 data_8003f9d0_slot28[];

void release_active_objects_in_table_8_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051a98_slot28[i];
        if (p != 0 && p->active_count != 0) {
            reset_and_push_object_to_pool_table((Slab172 *)p);
            data_80051a98_slot28[i] = 0;
        }
    }
}

void copy_two_tables_to_row_pairs_4_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_8003f184_slot28[i];
        data_801a27e4_rows[5][i] = data_8003f184_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_8003f584_slot28[i];
        data_801a27e4_rows[7][i] = data_8003f584_slot28[i];
    }
}

void call_handler_by_state_0_10_slot28(Object *object) {
    data_8003f9c0_slot28[object->state_0](object);
}

void init_object_and_start_indexed_sequence_2_slot28(Object *o) {
    o->field_0e = 0;
    o->field_0c = 0;
    o->facing_side = 0;
    o->variant_index = 0;
    o->state_0++;
    set_sequence_from_table_5(o, data_8003f9d0_slot28[o->table_index], (SequenceStep **)o->box_tables);
}
