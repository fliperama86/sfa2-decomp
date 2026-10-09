/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80141e5c(Object *object);

void func_801b3538_slot04_05(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    obj->field_45 = 1;
    obj->field_0b = 0;
    if ((obj->field_c2 & 0x8000) == 0) {
        obj->field_0b = 1;
        obj->field_4c = -0x20000;
    } else {
        obj->field_4c = 0x20000;
    }
    obj->field_50 = 0x60000;
    obj->field_54 = 0;
    obj->field_58 = -0x6000;
    func_801307e0(obj, 0x18);
    func_80141e5c(obj);
}

void func_801b35d8_slot04_05(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 5, 0xf, 0, 0, 1);
    } else {
        func_80141e5c(obj);
    }
}
