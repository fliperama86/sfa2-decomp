/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b18e8_slot04_07(Object *obj) {
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
        obj->field_159 = one;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}
