/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051a18_slot28[];
extern u16 data_8003be8c_slot28[];
extern u16 data_8003b68c_slot28[];
extern u16 data_8003ba8c_slot28[];
extern void (*data_8003c2c4_slot28[])(Object *);

void release_active_objects_in_table_6_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051a18_slot28[i];
        if (p != 0 && p->active_count != 0) {
            reset_and_push_object_to_pool_table((Slab172 *)p);
            data_80051a18_slot28[i] = 0;
        }
    }
}

void copy_three_word_tables_to_row_pairs_6_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_8003be8c_slot28[i];
        data_801a27e4_rows[5][i] = data_8003be8c_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_8003b68c_slot28[i];
        data_801a27e4_rows[7][i] = data_8003b68c_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[3][i] = data_8003ba8c_slot28[i];
        data_801a27e4_rows[8][i] = data_8003ba8c_slot28[i];
    }
}

void call_handler_by_state_0_9_slot28(Object *object) {
    data_8003c2c4_slot28[object->state_0](object);
}
