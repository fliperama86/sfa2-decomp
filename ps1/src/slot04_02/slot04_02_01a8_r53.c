/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6cb0_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b63f8_slot04_02(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0) {
        if ((u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
            func_801b6cb0_slot04_02(obj, 1, 2, 0, 0);
            return;
        }
    }
    if (obj->field_12a == 2 && obj->field_219 != 0) {
        obj->field_159 = 1;
        obj->field_07 = 2;
        func_80141f28(obj, 1);
        func_801307e0(obj, 0x27);
        return;
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b64d8_slot04_02(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        if (obj->field_128 == 2) {
            func_80131468(obj);
        } else {
            func_801312b8(obj);
        }
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}
