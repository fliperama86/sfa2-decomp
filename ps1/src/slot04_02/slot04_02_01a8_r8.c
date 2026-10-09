/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

u8 func_801b0d74_slot04_02(Object *obj) {
    u8 r = func_801417cc(obj);

    if (r) {
        func_801b631c_slot04_02(obj, 1, 0, 7, 0);
        obj->field_15a = 0xd;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}

u8 func_801b0de0_slot04_02(Object *obj) {
    u8 r;

    if (obj->field_7e == 0 && obj->field_177 == 0) return;
    r = func_801417cc(obj);
    if (r) {
        func_801b631c_slot04_02(obj, 1, 0, 7, 0);
        obj->field_15a = 0xc;
        obj->field_0b = obj->field_158;
    }
    return r;
}
