/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1510_slot04_04(Object *obj);
u8 func_80141788(Object *object);

u8 func_801b0f64_slot04_04(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        func_801b1510_slot04_04(obj);
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 3;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}

u8 func_801b0fe8_slot04_04(Object *obj) {
    int r = 0;

    if ((s16)obj->field_c6 < 0x30) {
        return 0;
    }
    if (func_80141788(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 4;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142778(obj);
    }
    return r;
}
