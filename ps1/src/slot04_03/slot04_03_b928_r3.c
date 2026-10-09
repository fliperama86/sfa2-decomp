/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0f44_slot04_03(Object *obj);

int func_801b0c90_slot04_03(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        func_801b0f44_slot04_03(obj);
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

