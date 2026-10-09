/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b13ec_slot04_09(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 2;
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

int func_801b1474_slot04_09(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 0xa;
        obj->field_159 = 1;
        obj->field_12c = 0;
        obj->field_12d = 0;
        obj->field_12e = 0;
        obj->field_12f = 0;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}
