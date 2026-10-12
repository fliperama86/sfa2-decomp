/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void release_object_and_unlink_from_parent_slot04_0b(Object *o, Object *unused) {
    ref_other.p = o->parent;
    ref_other.p->link_flags--;
    if (ref_other.p->link_flags == 0) {
        ref_other.p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}
