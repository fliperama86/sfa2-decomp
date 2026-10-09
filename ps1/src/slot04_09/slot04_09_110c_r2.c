/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1280_slot04_09(Object *obj) {
    obj->field_04 = 1;
    obj->field_06 = 7;
    obj->field_15a = 8;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x10;
    obj->field_12a = 0;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
}

u8 func_801417cc(Object *object);

int func_801b12c4_slot04_09(Object *obj) {
    int r = 0;

    if (obj->field_7e == 0) {
        if (obj->field_240 != 0) {
            return 0;
        }
    }
    if (func_801417cc(obj)) {
        r++;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 0;
        obj->field_159 = 1;
        obj->field_12c = 0;
        obj->field_12d = 0;
        obj->field_12e = 0;
        obj->field_12f = 0;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}

int func_801b1368_slot04_09(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 1;
        obj->field_159 = 1;
        obj->field_12c = 0;
        obj->field_12d = 0;
        obj->field_12e = 0;
        obj->field_12f = 0;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}
