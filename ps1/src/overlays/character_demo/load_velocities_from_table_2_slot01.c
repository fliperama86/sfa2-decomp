/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void unlink_from_parent_list_and_release_slot01(Object *obj);

void load_velocities_from_table_2_slot01(Object *obj) {
    unlink_from_parent_list_and_release_slot01(obj);
}
