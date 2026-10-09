/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b08c4_slot04_02(Object *obj);

void func_801b0878_slot04_02(Object *obj) {
    obj->field_50 = -0x4c000;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_0b != 0) {
        obj->field_4c = 0x40000;
    } else {
        obj->field_4c = -0x40000;
    }
    func_801b08c4_slot04_02(obj);
}

void func_801b08c4_slot04_02(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 -= obj->field_50;
    if (obj->field_70 <= (u16)obj->pos_y) {
        func_801209c4(obj);
        obj->pos_y = (u16)obj->field_70;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
