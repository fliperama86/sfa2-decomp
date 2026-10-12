/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80143b3c(Object *object) {
    int t;
    if (object->variant_index_2 == 7) data_8018f598 = 0;
    t = object->countdown - 1;
    object->countdown = t;
    if ((s16)t < 0) {
        object->field_0c = 1;
        object->image_row = 0xd;
        object->state_1 = object->state_1 + 1;
    }
}

void func_80143b90(Object *object) {
    object->pos_y = object->pos_y - 5;
    object->health = object->health + 3;
    if (object->table_index == 0) {
        object->pos_x = object->pos_x - 0x13;
        if (object->pos_x < -0x42) goto inc;
    } else {
        object->pos_x = object->pos_x + 0x13;
        if (object->pos_x >= 0x1c3) {
inc:
            object->state_1 = object->state_1 + 1;
        }
    }
}
