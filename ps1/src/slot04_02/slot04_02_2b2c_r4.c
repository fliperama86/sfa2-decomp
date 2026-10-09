/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2fb0_slot04_02(Object *obj) {
    obj->field_249 = 2;
    if (obj->field_4c >= 0) {
        if (*(u8 *)&obj->field_3a == 0) {
            obj->field_45 = 1;
            *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
            obj->field_50 = obj->field_50 + obj->field_58;
        }
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        }
        obj->field_4c = obj->field_4c + obj->field_54;
    } else {
        obj->field_07++;
    }
    func_80130efc(obj);
}
