/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80143864(Object *object) {
    object->pos_y = object->pos_y + 5;
    object->field_5c = object->field_5c - 3;
    if (object->field_03 == 0) {
        object->pos_x = object->pos_x - 0x13;
        if (object->pos_x < 0xc1) {
            object->pos_x = 0xc0;
            object->pos_y = 0x70;
            object->field_05 = object->field_05 + 1;
            object->field_5c = 0;
            object->field_0d = 0;
            if (object->field_a0 == 7) {
                data_8018f598 = 1;
            }
        }
    } else {
        object->pos_x = object->pos_x + 0x13;
        if (object->pos_x >= 0xc0) {
            object->pos_x = 0xc0;
            object->pos_y = 0x70;
            object->field_05 = object->field_05 + 1;
            object->field_5c = 0;
            object->field_0d = 0;
            if (object->field_a0 == 2) func_80120554(0, 0, 0x200);
            if (object->field_a0 == 3) func_80120554(0, 0, 0x201);
            if (object->field_a0 == 4) func_80120554(0, 0, 0x202);
            if (object->field_a0 == 5) func_80120554(0, 0, 0x203);
            if (object->field_a0 == 6) func_80120554(0, 0, 0x204);
        }
    }
}
