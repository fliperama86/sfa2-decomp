/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b03ac_slot04_02(Object *obj) {
    s16 t = obj->field_3a;

    if (t < 0) {
        func_801312b8(obj);
    } else {
        if ((t & 0xff) != 0) {
            if (obj->field_0b != 0) {
                *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + 0x20000;
            } else {
                *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - 0x20000;
            }
        }
        func_80130efc(obj);
    }
}
