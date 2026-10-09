/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int index);
u8 func_801b2510_slot04_0b(Object *obj);

void func_801b037c_slot04_0b(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80130678(obj, 0x30);
}

void func_801b03a8_slot04_0b(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_07 = obj->field_07 + 1;
        func_80130678(obj, 0x10);
    } else {
        func_80130efc(obj);
    }
}

void func_801b03f8_slot04_0b(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_50 = 0x80000;
        obj->field_54 = 0;
        obj->field_58 = -0x9000;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_0b == 0) {
            obj->field_4c = 0x28000;
        } else {
            obj->field_4c = -0x28000;
        }
        obj->field_45 = 1;
        func_80130678(obj, 0x14);
    } else {
        func_80130efc(obj);
    }
}

void func_801b047c_slot04_0b(Object *obj) {
    if (func_801b2510_slot04_0b(obj) == 0) {
        obj->field_45 = 0;
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = obj->field_70;
        func_801209c4(obj);
        func_80130678(obj, 0x11);
    } else {
        func_80130efc(obj);
    }
}
