/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b55b8_slot04_02(Object *obj) {
    if ((u8)obj->field_3a == 0) {
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        }
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
        obj->field_50 = obj->field_50 + obj->field_58;
        if (obj->pos_y >= obj->field_70) {
            obj->field_45 = 0;
            obj->pos_y = obj->field_70;
            func_801209c4(obj);
            func_801312b8(obj);
            return;
        }
    }
    func_80130efc(obj);
}
