/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801c5758_slot04_06;

void release_object_unlink_global_link_slot04_06(Object *o) {
    Object *p = data_801c5758_slot04_06;

    if ((s32)o == p->linked_object) {
        p->linked_object = 0;
    }
    clear_object_and_push_on_free_stack(o);
}
