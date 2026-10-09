/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b57a8_slot04_11(Object *obj) {
    s32 a = obj->field_4c;

    if (obj->field_0b == 0) {
        a = -a;
    }
    *(s32 *)&obj->field_10 = a + *(s32 *)&obj->field_10;
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_4c = 0;
        obj->field_54 = 0;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    return obj->field_50 = obj->field_50 + obj->field_58;
}
