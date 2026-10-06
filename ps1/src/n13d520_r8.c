/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80143b3c(Object *object) {
    int t;
    if (object->field_a0 == 7) data_8018f598 = 0;
    t = object->field_46 - 1;
    object->field_46 = t;
    if ((s16)t < 0) {
        object->field_0c = 1;
        object->field_0d = 0xd;
        object->field_05 = object->field_05 + 1;
    }
}

void func_80143b90(Object *object) {
    object->pos_y = object->pos_y - 5;
    object->field_5c = object->field_5c + 3;
    if (object->field_03 == 0) {
        object->pos_x = object->pos_x - 0x13;
        if (object->pos_x < -0x42) goto inc;
    } else {
        object->pos_x = object->pos_x + 0x13;
        if (object->pos_x >= 0x1c3) {
inc:
            object->field_05 = object->field_05 + 1;
        }
    }
}
