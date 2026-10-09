/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b16a0_slot04_00(Object *obj) {
    if (obj->field_49 != 0 && obj->field_50 < 0) {
        obj->field_58 = 0xffff0000;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
}
