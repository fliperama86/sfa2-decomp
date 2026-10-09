/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4390_slot04_01(Object *obj) {
    s16 t = obj->field_3a;
    int d = 0x20000;

    if ((t & 0x8000) != 0) {
        func_801312b8(obj);
    } else {
        if ((t & 0xff) != 0) {
            if (obj->field_0b == 0) {
                *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - d;
            } else {
                *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + d;
            }
        }
        func_80130efc(obj);
    }
}
