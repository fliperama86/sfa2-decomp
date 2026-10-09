/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b4be8_slot04_04(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    obj->field_4c = obj->field_4c + obj->field_54;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    return obj->field_50 = obj->field_50 + obj->field_58;
}
