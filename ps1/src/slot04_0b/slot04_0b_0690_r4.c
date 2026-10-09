/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1150_slot04_0b(Object *obj) {
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
