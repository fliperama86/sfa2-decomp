/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6548_slot04_14[];
extern ObjectFn data_801c6554_slot04_14[];

void func_801b6344_slot04_14(Object *obj);
void func_801b657c_slot04_14(Object *obj);
void func_801b6c3c_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_80130dc0(Object *object);

void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->field_04 = a;
    obj->field_05 = b;
    obj->field_06 = c;
    obj->field_07 = d;
}

void func_801b62c0_slot04_14(Object *obj) {
    data_801c6548_slot04_14[obj->field_128 >> 1](obj);
}

void func_801b6304_slot04_14(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b657c_slot04_14(obj);
    } else {
        func_801b6344_slot04_14(obj);
    }
}

void func_801b6344_slot04_14(Object *obj) {
    data_801c6554_slot04_14[obj->field_07](obj);
}

void func_801b6384_slot04_14(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0) {
        if ((u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
            func_801b6c3c_slot04_14(obj, 1, 2, 0, 0);
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
