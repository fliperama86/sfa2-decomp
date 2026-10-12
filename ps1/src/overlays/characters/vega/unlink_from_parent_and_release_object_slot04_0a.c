/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void unlink_from_parent_and_release_object_slot04_0a(Object *obj) {
    ref_other.p = obj->parent;
    ref_other.p->link_flags--;
    if (ref_other.p->link_flags & 0x80) {
        ref_other.p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)obj);
}
