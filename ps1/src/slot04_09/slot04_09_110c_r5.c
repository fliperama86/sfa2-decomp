/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1644_slot04_09(Object *obj) {
    int r = 0;

    if ((s16)obj->field_c6 >= 0x30) {
        if (obj->field_246 == 0) {
            if ((u8)func_80141788(obj)) {
                r = 1;
                obj->state_0 = 1;
                obj->state_2 = 8;
                obj->state_1 = 0;
                obj->state_3 = 0;
                obj->field_15a = 6;
                obj->field_159 = 0;
                obj->field_12c = 0;
                obj->field_12d = 0;
                obj->field_12e = 0;
                obj->field_12f = 0;
                obj->field_0b = obj->field_158;
                func_80142778(obj);
            }
        }
    }
    return r;
}
