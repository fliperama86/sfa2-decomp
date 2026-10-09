/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b0d54_slot04_01(Object *obj) {
    int r = 0;

    if (obj->field_45 != 0) {
        if (!func_801418bc(obj)) return 0;
        r = 1;
        obj->field_15a = 3;
    } else {
        if (!func_801417cc(obj)) return 0;
        r = 1;
        obj->field_15a = 2;
        obj->field_0b = obj->field_158;
    }
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_159 = 1;
    func_80142ba0(obj);
    return r;
}
