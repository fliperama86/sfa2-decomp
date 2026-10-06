/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014a648(Object *object) {
    u16 value;
    object->field_208 = 0;
    object->field_209 = 0;
    value = *data_80189460++;
    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 0;
    object->field_07 = 0;
    object->field_21c = value;
}
