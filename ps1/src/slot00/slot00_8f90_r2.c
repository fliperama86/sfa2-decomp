/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void unlink_from_parent_slot_and_release_slot00(Object *o) {
    Slot00Obj *t = (Slot00Obj *)o->parent;
    if (o->table_index == 0) {
        if (o == t->linked_object_a) {
            t->linked_object_a = 0;
        }
    } else {
        if (o == t->linked_object_b) {
            t->linked_object_b = 0;
        }
    }
    func_8011f38c(o);
}
