/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void set_hit_type_from_box_type_with_flag(Object *a, Object *object, Box32 *c) {
    object->field_15b = 1;
    object->hit_type = c->box_type;
    if (object->hit_type == 0x1d) {
        object->hit_type = 0x12;
    }
}
