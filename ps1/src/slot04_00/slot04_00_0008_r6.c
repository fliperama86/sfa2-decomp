/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0764_slot04_00(Object *obj) {
    if ((u8)obj->field_3a == 0) {
        obj->field_45 = 1;
        *(s32 *)&obj->field_10 += obj->field_4c;
        *(s32 *)&obj->field_14 -= obj->field_50;
        obj->field_50 += obj->field_58;
        if (obj->pos_y >= obj->field_70) {
            obj->field_07 = obj->field_07 + 1;
            obj->field_45 = 0;
            obj->field_159 = 0;
            obj->pos_y = (u16)obj->field_70;
            func_801209c4(obj);
            func_801307e0(obj, 0x30);
            return;
        }
    }
    func_80130efc(obj);
}

void func_801b081c_slot04_00(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->other->field_249 = 6;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
