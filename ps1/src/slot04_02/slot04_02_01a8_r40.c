/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4e68_slot04_02(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a == 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
        obj->field_50 = obj->field_50 + obj->field_58;
        if (obj->pos_y >= obj->field_70) {
            obj->field_07 = obj->field_07 + 1;
            obj->field_45 = 0;
            obj->pos_y = obj->field_70;
            func_801209c4(obj);
            func_801307e0(obj, 0x4b);
        }
    }
}
