/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_1_if_variant_set_3_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_1 += 1;
    }
}
