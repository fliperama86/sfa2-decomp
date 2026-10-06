/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012de9c(Object *object) {
    int v;
    object->field_48 = 0xff;
    object->field_45 = 0xff;
    object->field_50 = 0x10000;
    object->field_58 = -0x6800;
    object->field_46 = 0;
    object->field_54 = 0;
    object->field_07 = object->field_07 + 1;
    if (object->field_72 == 0) {
        v = -0x60000;
    } else {
        v = 0x60000;
    }
    object->field_4c = v;
}
