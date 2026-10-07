/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b390c_slot04_00(Object *obj) {
    s16 v = obj->field_3a;
    if (v & 0x8000) {
        func_801312b8(obj);
    } else {
        if (v & 0xff) {
            if (obj->field_0b == 0) {
                *(s32 *)&obj->field_10 += -0x20000;
            } else {
                *(s32 *)&obj->field_10 += 0x20000;
            }
        }
        func_80130efc(obj);
    }
}
