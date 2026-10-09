/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80141788(Object *object);

int func_801b0f78_slot04_09(Object *obj) {
    int r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->field_c6 >= 0x30) {
            if (obj->field_45 == 0) {
                if ((u8)func_80141e34(obj)) {
                    if (func_80141788(obj)) {
                        r = 1;
                        obj->field_04 = 1;
                        obj->field_05 = 0;
                        obj->field_06 = 8;
                        obj->field_07 = 0;
                        obj->field_15a = 9;
                        obj->field_159 = 1;
                    }
                }
            } else if (obj->field_7e == 0) {
                if (func_801418bc(obj)) {
                    r = 1;
                    obj->field_04 = 1;
                    obj->field_05 = 0;
                    obj->field_06 = 8;
                    obj->field_07 = 0;
                    obj->field_15a = 9;
                }
            }
        }
    }
    return r;
}

int func_801b1070_slot04_09(Object *obj) {
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
        obj->field_159 = 0;
        obj->field_12c = 0;
        obj->field_12d = 0;
        obj->field_12e = 0;
        obj->field_12f = 0;
        obj->field_0b = obj->field_158;
    }
    return r;
}
