/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b05a4_slot04_02(Object *obj) {
    if ((u8)obj->field_3a == 0) {
        obj->field_45 = 1;
        *(s32 *)&obj->field_10 += obj->field_4c;
        *(s32 *)&obj->field_14 -= obj->field_50;
        obj->field_50 += obj->field_58;
        if (obj->field_70 <= obj->pos_y) {
            obj->field_07 = obj->field_07 + 1;
            obj->field_159 = 0;
            obj->field_45 = 0;
            obj->pos_y = (u16)obj->field_70;
            func_801209c4(obj);
            func_801307e0(obj, 0x41);
            return;
        }
    }
    func_80130efc(obj);
}
