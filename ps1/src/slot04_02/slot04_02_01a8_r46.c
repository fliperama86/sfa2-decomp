/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b5ef0_slot04_02(Object *obj);

void func_801b5b44_slot04_02(Object *obj) {
    Object *o = obj->other;
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(o, o->side, 0x31a);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if (func_801b5ef0_slot04_02(obj) != 0) {
            obj->field_0b = obj->field_0b + 1;
        }
    } else if (obj->field_c2 & 0x8000) {
        obj->field_0b = 1;
    }
    func_801307e0(obj, (obj->field_129 >> 1) + 0x18);
}

void func_801b5bf4_slot04_02(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 5, 0xf, 0, 1, 0);
    }
    func_80130efc(obj);
}

void func_801b5c58_slot04_02(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
