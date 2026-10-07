/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5118_slot04_07(Object *o) {
    Slot04aObj *q = (Slot04aObj *)o;
    Slot04aObj *obj = (Slot04aObj *)o;
    int t;
    if ((obj->field_45 & 1) == 0) {
        t = obj->field_4c;
        t <<= 8;
        *(s32 *)&obj->field_5c += t;
        t = obj->field_50;
        obj->field_4c += obj->field_54;
        obj->field_50 += obj->field_58;
        t <<= 8;
        obj->field_60 -= t;
    }
    if ((q->field_45 & 2) == 0) {
        t = q->field_4e;
        t <<= 8;
        *(s32 *)&q->field_6c += t;
        t = q->field_52;
        q->field_4e += q->field_56;
        q->field_52 += q->field_5a;
        t <<= 8;
        *(s32 *)&q->field_70 -= t;
    }
}
