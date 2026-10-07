/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b20d4_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u16 a;

    *(s32 *)&o->field_10 += o->field_4c;
    *(s32 *)&o->field_14 -= o->field_50;
    o->field_50 += o->field_58;
    if (o->field_50 < 0) {
        o->field_07++;
        func_80120554(o, o->side, 0x320);
        if (o->field_49 != 0) {
            a = 0x70;
        } else {
            a = 0x20;
        }
        if (o->field_12a != 0) {
            obj->field_1a4 = o->field_12a;
        } else {
            obj->field_1a4 = 1;
        }
        a += o->field_12a >> 1;
        func_801307e0(o, a);
    } else {
        func_80130efc(o);
    }
}
