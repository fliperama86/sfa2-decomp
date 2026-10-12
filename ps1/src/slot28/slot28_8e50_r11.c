/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_0_on_variant_index_set_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_0++;
    }
}
