/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void unlink_from_parent_and_set_sequence_9_slot04_0b(Object *o, Object *unused) {
    o->state_1++;
    ref_other.p = o->parent;
    ref_other.p->linked_object_3 = 0;
    set_sequence_by_sound_side(o, 9);
}
