/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051890_slot28;

void increment_substep_and_release_object_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        data_8018f5a0->substep_index++;
        reset_and_push_object_to_pool_table((Slab172 *)data_80051890_slot28.p);
    }
}
