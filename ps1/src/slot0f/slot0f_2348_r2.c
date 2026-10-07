/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800e8548_slot0f[];
extern u8 data_800e8578_slot0f[];
extern Slot0fRec85c8 data_800e85c8_slot0f[];

void func_800e2498_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    int i;
    int c;
    Slot0fRec85c8 *r;
    if ((s8)obj->field_00 == 2) {
        func_801519b4((Object *)data_800e8548_slot0f);
    } else {
        func_801519b4((Object *)data_800e8578_slot0f);
    }
    for (i = 0; i < 3; i++) {
        r = &data_800e85c8_slot0f[i];
        c = 0x1a;
        if (i != 2) {
            if (!(obj->field_0d & (i + 1))) {
                c = 0x1b;
            }
        }
        if (i == obj->field_0b) {
            c = 0x10;
        }
        r->field_0b = c;
        func_801519b4((Object *)r);
    }
}

void func_800e2550_slot0f(Object *o) {
    if (o->field_10 & 0x5000) {
        if (o->field_10 & 0x1000) {
            ((Slot0fObj *)o)->field_0c--;
            if (((Slot0fObj *)o)->field_0c < 0) {
                ((Slot0fObj *)o)->field_0c = 1;
            }
        } else {
            ((Slot0fObj *)o)->field_0c++;
            if (((Slot0fObj *)o)->field_0c >= 2) {
                ((Slot0fObj *)o)->field_0c = 0;
            }
        }
        func_80120554((Object *)0, 0, 0x204);
    }
}
