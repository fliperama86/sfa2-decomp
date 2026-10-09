/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



int func_80130184(Object *object) {
    *(s32 *)&object->field_10 += object->field_4c;
    object->field_4c += object->field_54;
    *(u32 *)&object->field_14 -= object->field_50;
    object->field_50 += object->field_58;
    return object->pos_y <= object->field_70;
}

void func_801301d8(Object *object) {
    if (object->field_48 != 0) {
        if (object->field_130 & 0x8000) func_8012f838(object);
    } else {
        if (object->field_130 & 0x2000) func_8012f838(object);
    }
    *(s32 *)&object->field_10 += object->field_4c;
}
