/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);

int func_801b0e04_slot04_01(Object *obj) {
    int r = 0;

    if (func_801417cc(obj) != 0) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 0xa;
        obj->field_0b = obj->field_158;
    }
    return r;
}

int func_801b0e70_slot04_01(Object *obj) {
    int r = 0;

    if (obj->field_7e == 0) {
        if (obj->field_177 == 0) {
            return 0;
        }
    }
    if (func_801417cc(obj) != 0) {
        r++;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 7;
        obj->field_0b = obj->field_158;
    }
    return r;
}
