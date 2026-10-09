/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b0de0_slot04_04(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 0;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}

u8 func_801b0e54_slot04_04(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 1;
        obj->field_159 = 1;
        obj->field_67 = 0;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}

u8 func_801b0ecc_slot04_04(Object *obj) {
    u8 r = 0;

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
        obj->field_15a = 2;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}
