/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013ad2c(Object *object) {
    u8 v = table_8017727c[object->field_61][0];

    object->field_15b = 1;
    object->field_163 = 1;
    object->field_15d = 0;
    object->field_15e = 0;
    object->field_61 = v;
}

void func_8013ad64(Object *object) {
    handlers_72f4[object->field_61](object);
}
