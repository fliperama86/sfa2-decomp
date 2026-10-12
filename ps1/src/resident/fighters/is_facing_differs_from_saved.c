/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


int is_facing_differs_from_saved(Object *object) {
    if (object->field_7e == 0 && object->saved_facing != object->facing_side) return 1;
    return 0;
}
