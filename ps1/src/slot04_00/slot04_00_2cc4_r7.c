/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_parent_link_and_set_sequence_6_slot04_00(Object *obj) {
    obj->state_1 = obj->state_1 + 1;
    ref_other.p = obj->parent;
    ref_other.p->linked_object_3 = 0;
    set_sequence_by_sound_side(obj, 6);
}
