/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3cf8_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int v;

    obj->field_1c6 -= 1;
    if (obj->field_1c6 & 0x80) {
        o->field_07++;
        func_801307e0(o, 0x33);
    } else {
        v = 0x80000;
        if (o->field_0b == 0) {
            v = -0x80000;
        }
        *(s32 *)&o->field_10 = v + *(s32 *)&o->field_10;
        func_80130efc(o);
    }
}
