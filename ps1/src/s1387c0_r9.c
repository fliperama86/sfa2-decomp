/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void remap_hit_type_from_table(Object *object) {
    u8 v = table_8017727c[object->hit_type][0];

    object->field_15b = 1;
    object->field_163 = 1;
    object->countdown_timer_2 = 0;
    object->accum_count = 0;
    object->hit_type = v;
}

void call_handler_by_hit_type(Object *object) {
    handlers_72f4[object->hit_type](object);
}
