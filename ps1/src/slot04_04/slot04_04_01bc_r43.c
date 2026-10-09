/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b3a40_slot04_04(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    obj->field_4c = obj->field_4c + obj->field_54;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    return obj->field_50 = obj->field_50 + obj->field_58;
}

int func_801b3a84_slot04_04(Object *obj) {
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_4c = 0;
        obj->field_54 = 0;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    return obj->field_50 += obj->field_58;
}
