/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014b0e4(Object *object) {
    u16 value = *data_80189460++;
    data_80189464 = value;
    object->field_21c = value;
    object->field_209 = 4;
    object->field_04 = object->field_208 = 1;
    object->field_05 = 0;
    object->field_06 = 2;
    object->field_07 = 0;
    object->field_21a = object->field_48 = 1;
}
