/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800e8790_slot0f[];
extern void (*data_800e8794_slot0f[])(Object *);

void func_800e171c_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    u16 b = o->field_10;
    if (b & 0x5000) {
        if (b & 0x1000) {
            obj->field_0a -= 1;
            if (obj->field_0a < 0) {
                o->field_0a = 2;
            }
        } else {
            o->field_0a += 1;
            if ((s8)o->field_0a >= 3) {
                o->field_0a = 0;
            }
        }
        func_80120554(0, 0, 0x204);
        b = o->field_10;
    }
    if (b & 0x860) {
        if (b & 0x820) {
            o->field_00 = data_800e8790_slot0f[(s8)o->field_0a];
            func_80120554(0, 0, 0x205);
        } else {
            o->field_00 = 5;
            func_80120554(0, 0, 0x202);
        }
        o->field_03 = 0;
        o->field_02 = 0;
        o->field_01 = 0;
        o->field_07 = 0;
        o->field_06 = 0;
        o->field_05 = 0;
        o->field_04 = 0;
    }
}

void func_800e182c_slot0f(Object *o) {
    data_800e8794_slot0f[(s8)o->field_01](o);
}
