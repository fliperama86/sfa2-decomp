/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801567e4(void *a, Object *object) {
    int t = object->field_b0 - 1;
    object->field_b0 = t;
    if (t << 16 < 0) {
        object->field_aa = object->field_aa + 1;
    }
}
