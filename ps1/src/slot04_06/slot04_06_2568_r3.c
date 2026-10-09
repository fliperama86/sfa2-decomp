/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b27a4_slot04_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;
    s32 a;

    s->field_1c2 = s->field_1c2 - 1;
    if (s->field_1c2 & 0x80) {
        obj->field_07 = obj->field_07 + 1;
        func_801204f4(obj, obj->side, 5);
        obj->field_50 = -0x100000;
        obj->field_4c = 0;
        obj->field_54 = 0;
        func_801307e0(obj, 0x24);
    } else {
        a = 0x4000;
        if (obj->field_0b == 0) {
            a = -0x4000;
        }
        *(s32 *)&obj->field_10 += a;
        *(s32 *)&obj->field_14 += -0x4000;
        func_80130efc(obj);
    }
}
