/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void set_hit_type_7(Object *object) {
    object->hit_type = 7;
    object->field_62 = 2;
    object->field_15b = 1;
}

void set_hit_type_0c_state(Object *object) {
    object->hit_type = 0xc;
    object->field_62 = 2;
    object->field_15b = 1;
}

void set_hit_type_0d_state(Object *object) {
    object->hit_type = 0xd;
    object->field_62 = 2;
    object->field_15b = 1;
}

void noop_object_handler_2(Object *unused) {
}
