/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6560_slot04_02(Object *obj) {
    s16 t = obj->field_3a;
    u8 a;

    if (t & 0x8000) {
        func_801312b8(obj);
        return;
    }
    a = t;
    if (a != 0) {
        if (a == 2) {
            obj->field_3a = (t & -0x100) | 1;
        }
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 += 0x20000;
        } else {
            *(s32 *)&obj->field_10 -= 0x20000;
        }
    }
    func_80130efc(obj);
}
