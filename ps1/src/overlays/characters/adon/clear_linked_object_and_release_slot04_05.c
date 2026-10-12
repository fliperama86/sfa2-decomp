/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void clear_linked_object_and_release_slot04_05(Object *o, Object *p) {
    if ((s32)o == p->linked_object) {
        p->linked_object = 0;
    }
    clear_object_and_push_on_free_stack(o);
}
