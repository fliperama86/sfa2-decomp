/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80141b28(Object *object);
void func_80142ba0(Object *object);

int func_801b110c_slot04_09(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        obj->field_15a = 3;
        obj->field_159 = 1;
        obj->field_04 = 1;
        obj->field_06 = 7;
        obj->field_05 = 0;
        obj->field_07 = 0;
        obj->field_6b = 0;
        obj->other->field_6b = 0xc;
        obj->field_12c = 0;
        obj->field_12d = 0;
        obj->field_12e = 0;
        obj->field_12f = 0;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}

int func_801b11a4_slot04_09(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        obj->field_15a = 8;
        obj->field_159 = 1;
        obj->field_04 = 1;
        obj->field_06 = 7;
        obj->field_05 = 0;
        obj->field_07 = 0;
        obj->field_6b = 0;
        obj->other->field_6b = 0x10;
        obj->field_12c = 0;
        obj->field_12d = 0;
        obj->field_12e = 0;
        obj->field_12f = 0;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}

void func_801b123c_slot04_09(Object *obj) {
    obj->field_04 = 1;
    obj->field_06 = 7;
    obj->field_15a = 3;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0xc;
    obj->field_12a = 0;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
}
