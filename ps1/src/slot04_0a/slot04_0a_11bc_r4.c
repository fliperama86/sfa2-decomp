/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e44_slot04_0a(Object *obj);

int func_801b1608_slot04_0a(Object *obj) {
    int r = 0;

    if (func_801417cc(obj) != 0) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 6;
        func_801b3e44_slot04_0a(obj);
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}
