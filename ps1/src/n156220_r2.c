/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801565b8(void *a, Object *object) {
    s16 t = *(s16 *)&object->field_b0;
    if (t != 0) {
        object->field_b0 = t - 1;
    } else {
        object->field_b0 = 600;
        object->field_aa = object->field_aa + 1;
    }
}
