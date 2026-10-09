/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6c40_slot04_02(Object *obj);

void func_801b6bf4_slot04_02(Object *obj) {
    obj->field_50 = -0x4c000;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_0b != 0) {
        obj->field_4c = 0x40000;
    } else {
        obj->field_4c = -0x40000;
    }
    func_801b6c40_slot04_02(obj);
}

void func_801b6c40_slot04_02(Object *obj) {
    s16 t;

    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    t = obj->field_70;
    if (t <= obj->pos_y) {
        obj->pos_y = t;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b6cb0_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->field_04 = a;
    obj->field_05 = b;
    obj->field_06 = c;
    obj->field_07 = d;
}
