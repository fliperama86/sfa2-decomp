/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80141788(Object *object);

int func_801b0ef8_slot04_00(Object *obj) {
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
        obj->field_15a = 1;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}

int func_801b0f8c_slot04_00(Object *obj) {
    int r = 0;
    u8 one;

    if (func_801417cc(obj) != 0) {
        r = 1;
        one = 1;
        obj->field_04 = one;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 9;
        obj->field_0b = obj->field_158;
    }
    return r;
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int func_801b0ff8_slot04_00(Object *obj) {
    if ((s16)obj->field_c6 >= 0x30) {
        if (obj->field_240 == 0) {
            if (func_80141788(obj)) {
                obj->field_04 = 1;
                obj->field_05 = 0;
                obj->field_06 = 8;
                obj->field_07 = 0;
                obj->field_15a = 4;
                obj->field_159 = 1;
                obj->field_0b = obj->field_158;
                func_80142718(obj);
            }
        }
    }
}
