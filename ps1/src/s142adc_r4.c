/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801439e8(Object *object) {
    if (object->field_a0 == 7) {
        if (object->field_a2++ == 2) {
            object->field_a2 = 0;
            object->field_01 = 0;
            func_801373e8();
            object->field_05 = object->field_05 + 1;
            if (object->field_a3 == 0) func_80120554(0, 0, 0x206);
        }
    } else {
        object->field_05 = object->field_05 + 1;
    }
}

void func_80143a84(Object *object) {
    object->field_0c = 0;
    if (object->field_a0 == 7) {
        if (object->field_a2++ == 2) {
            object->field_01 = 1;
            func_801374c0();
            if (object->field_a3 == 0) {
                object->field_05--;
                object->field_a2 = 1;
                object->field_a3 = 1;
                data_8018f598 = 1;
            } else {
                object->field_a2 = 0;
                object->field_a3 = 0;
                object->field_05 = object->field_05 + 1;
            }
        }
    } else {
        object->field_05 = object->field_05 + 1;
    }
}
