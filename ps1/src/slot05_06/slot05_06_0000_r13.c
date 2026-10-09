/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80141c4c(Object *object);
u8 func_801417cc(Object *object);

int func_801c904c_slot05_06(Object *obj) {
    if (!(u8)func_80141c4c(obj)) return 0;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 6;
    obj->field_159 = 1;
    obj->field_12a = 2;
    return 1;
}

int func_801c90b4_slot05_06(Object *obj) {
    if (obj->field_7e != 0 || obj->field_177 != 0) {
        if (func_801417cc(obj) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 0;
            obj->field_06 = 7;
            obj->field_07 = 0;
            obj->field_15a = 7;
            obj->field_159 = 1;
            obj->field_0b = obj->field_158;
            return 1;
        }
    }
    return 0;
}
