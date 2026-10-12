/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_0_clear_facing_and_variant_slot28(Object *obj) {
    obj->state_0 += 1;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
}
