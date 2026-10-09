/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b18a8_slot04_0b(Object *obj) {
    if (obj->field_50 < 0) {
        if (obj->field_49 == 0) {
            obj->field_58 = -0x8000;
        } else {
            obj->field_58 = -0x10000;
        }
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
}
