/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80144320(Object *object) {
    int t;
    *(int *)&object->field_10 = *(int *)&object->field_10 + object->field_4c;
    t = object->field_46 - 1;
    object->field_46 = t;
    if ((t & 0x80) != 0) {
        object->pos_x = 0xc0;
        object->field_06 = object->field_06 + 1;
    }
}
