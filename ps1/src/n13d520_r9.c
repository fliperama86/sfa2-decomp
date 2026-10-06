/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80143dcc(Object *object) {
    object->pos_x = object->pos_x - 9;
    if (object->pos_x < -0x42) object->field_05 = 6;
}

void func_80143dfc(Object *object) {
    int t = object->field_46 - 1;
    object->field_46 = t;
    if ((s16)t < 0) object->field_05 = object->field_05 + 1;
}

void func_80143e30(Object *object) {
    object->pos_x = object->pos_x - 9;
    if (object->pos_x < -0x42) object->field_05 = 6;
}
