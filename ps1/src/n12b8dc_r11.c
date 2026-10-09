/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013a320(Object *a, Object *object, Box32 *c) {
    object->field_15b = 1;
    object->field_61 = c->field_0d;
    if (object->field_61 == 0x1d) {
        object->field_61 = 0x12;
    }
}
