/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801449d4(Object *object) {
    if ((s16)object->field_3a < 0) {
        object->field_09 = 0;
        object->field_04 = object->field_04 + 1;
    }
}
