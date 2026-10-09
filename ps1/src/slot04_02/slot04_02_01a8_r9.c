/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b0fd8_slot04_02(Object *obj);

u8 func_801b0ef4_slot04_02(Object *obj) {
    u8 r = func_801417cc(obj);
    u8 t;

    if (r) {
        func_801b631c_slot04_02(obj, 1, 0, 7, 0);
        t = obj->field_158;
        obj->field_15a = 8;
        obj->field_0b = t;
        obj->field_48 = t ^ 1;
        func_801b0fd8_slot04_02(obj);
    }
    return r;
}

u8 func_801b0f68_slot04_02(Object *obj) {
    u8 r = func_801417cc(obj);

    if (r) {
        func_801b631c_slot04_02(obj, 1, 0, 7, 0);
        obj->field_15a = 8;
        obj->field_48 = obj->field_0b = obj->field_158;
        func_801b0fd8_slot04_02(obj);
    }
    return r;
}

void func_801b0fd8_slot04_02(Object *obj) {
    u16 t;

    t = obj->field_134 | obj->field_136;
    obj->field_129 = 0;
    obj->field_12a = 0;
    if ((t & 0x68) == 0x68 || (t & 2) != 0) {
        obj->field_129 = obj->field_129 + 2;
    }
}
