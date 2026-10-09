/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b567c_slot04_02(Object *obj) {
    u16 t;
    func_80130efc(obj);
    t = obj->field_3a;
    if ((u8)t != 0) {
        obj->field_3a = t & 0xff00;
        func_80120554(obj, obj->side, 0x320);
    }
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->pos_y >= obj->field_70) {
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        func_801307e0(obj, 0x5d);
    }
}
