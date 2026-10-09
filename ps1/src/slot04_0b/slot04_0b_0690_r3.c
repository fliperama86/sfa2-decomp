/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80141788(Object *object);

int func_801b0da4_slot04_0b(Object *obj) {
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
                        obj->field_15a = 0xa;
                        obj->field_0b = obj->field_158;
                    }
                }
            } else if (obj->field_7e == 0) {
                if (func_801418bc(obj)) {
                    r = 1;
                    obj->field_04 = 1;
                    obj->field_05 = 0;
                    obj->field_06 = 8;
                    obj->field_07 = 0;
                    obj->field_15a = 0xa;
                }
            }
        }
    }
    return r;
}
