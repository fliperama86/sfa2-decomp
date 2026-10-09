/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3074_slot04_02(Object *obj) {
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_50 < 0) {
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b30cc_slot04_02(Object *obj) {
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->pos_y < obj->field_70) {
        if ((u8)obj->field_3a != 0) {
            return;
        }
    } else {
        obj->field_159 = 0;
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
    }
    func_80130efc(obj);
}
