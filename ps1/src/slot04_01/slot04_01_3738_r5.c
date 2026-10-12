/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_parent_link_and_set_sequence_3_slot04_01(Object *obj) {
    obj->state_1++;
    obj->parent->linked_object_3 = 0;
    func_80138070(obj, 3);
}
