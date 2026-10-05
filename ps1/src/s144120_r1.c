/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void func_80144120(Object *object) {
    if (*(u8 *)&object->field_3a == 0) {
        func_80144220(object);
        object->field_46 = (s16)object->field_46 - 1;
        if (object->field_46 & 0x80) {
            object->pos_x = 0xc0;
            object->field_06 = object->field_06 + 1;
            func_80120554(0, 0, 0x20a);
        }
    }
}

void func_80144198(Object *object) {
    if ((s16)object->field_3a < 0) {
        func_80144e90(object);
    }
}
