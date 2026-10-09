/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b0a38_slot04_08(Object *obj) {
    int r = 0;
    u8 one;

    if (func_801417cc(obj) != 0) {
        r = 1;
        one = 1;
        obj->field_04 = one;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_129 = 6;
        obj->field_15a = one;
        obj->field_159 = one;
        obj->field_12a = ((Slot04aObj *)obj)->field_2d5;
        obj->field_0b = obj->field_158;
    }
    return r;
}

int func_801b0ab4_slot04_08(Object *obj) {
    int r = 0;
    u8 one;

    if (func_801417cc(obj) != 0) {
        r = 1;
        one = 1;
        obj->field_04 = one;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_129 = 0;
        obj->field_15a = one;
        obj->field_159 = one;
        obj->field_12a = ((Slot04aObj *)obj)->field_2bd;
        obj->field_0b = obj->field_158;
    }
    return r;
}

int func_801b0b2c_slot04_08(Object *obj) {
    int r = 0;
    u8 one;

    if (func_801417cc(obj) != 0) {
        r = 1;
        one = 1;
        obj->field_04 = one;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_129 = 2;
        obj->field_159 = one;
        obj->field_15a = one;
        obj->field_12a = ((Slot04aObj *)obj)->field_2c5;
        obj->field_0b = obj->field_158;
    }
    return r;
}
