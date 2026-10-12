/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051c64_slot28[];
extern u16 data_8004e610_slot28[];
extern u16 data_8004de10_slot28[];
extern u16 data_8004e210_slot28[];
extern void (*data_8004ea40_slot28[])(Object *);

void release_active_objects_in_table_15_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051c64_slot28[i];
        if (p != 0 && p->active_count != 0) {
            reset_and_push_object_to_pool_table((Slab172 *)p);
            data_80051c64_slot28[i] = 0;
        }
    }
}

void copy_three_word_tables_to_row_pairs_10_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_8004e610_slot28[i];
        data_801a27e4_rows[5][i] = data_8004e610_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_8004de10_slot28[i];
        data_801a27e4_rows[7][i] = data_8004de10_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[3][i] = data_8004e210_slot28[i];
        data_801a27e4_rows[8][i] = data_8004e210_slot28[i];
    }
}

void call_handler_by_state_0_18_slot28(Object *object) {
    data_8004ea40_slot28[object->state_0](object);
}
