/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b0c84_slot04_00(Object *obj) {
    int r = 0;

    if (obj->field_7e == 0 && obj->field_177 == 0) return 0;
    if (func_801417cc(obj)) {
        r++;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 6;
        obj->field_0b = obj->field_158;
    }
    return r;
}
