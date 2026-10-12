/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013ad2c(Object *object) {
    u8 v = table_8017727c[object->hit_type][0];

    object->field_15b = 1;
    object->field_163 = 1;
    object->countdown_timer_2 = 0;
    object->accum_count = 0;
    object->hit_type = v;
}

void func_8013ad64(Object *object) {
    handlers_72f4[object->hit_type](object);
}
