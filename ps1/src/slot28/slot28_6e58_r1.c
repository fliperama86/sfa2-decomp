/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800518d0_slot28[];

void clear_ten_entry_table_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_800518d0_slot28[i] = 0;
    }
}

void release_active_objects_in_table_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_800518d0_slot28[i];
        if (p != 0 && p->active_count != 0) {
            reset_and_push_object_to_pool_table((Slab172 *)p);
            data_800518d0_slot28[i] = 0;
        }
    }
}
