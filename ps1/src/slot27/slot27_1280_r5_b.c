/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void copy_side_palette_from_table_and_upload_slot27(Object *obj);

void release_object_slot27(Object *object) {
    reset_and_push_object_to_pool_table((Slab172 *)object);
}
