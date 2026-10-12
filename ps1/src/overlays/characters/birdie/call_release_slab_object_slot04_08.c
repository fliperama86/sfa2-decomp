/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_release_slab_object_slot04_08(Object *obj, Object *unused) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
