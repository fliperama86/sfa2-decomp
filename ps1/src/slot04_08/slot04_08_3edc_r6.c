/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_other_link_and_release_object_slot04_08(Object *obj, Object *other) {
    other->linked_object = 0;
    func_8011f38c(obj);
}
