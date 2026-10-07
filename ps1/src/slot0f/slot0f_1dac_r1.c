/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec8508 data_800e8508_slot0f;
extern Slot0fRec8508 data_800e8518_slot0f[];

extern ObjectFn data_800e87cc_slot0f[];

void func_800e1dac_slot0f(Object *obj) {
    s8 t;

    if (obj->field_10 & 0x860) {
        obj->field_0f = 1;
    }
    t = obj->field_0f;
    t--;
    obj->field_0f = t;
    if (t == 0) {
        obj->field_00 = 1;
        obj->field_01 = 0;
        obj->field_09 = 0;
        obj->field_04 = 0;
    }
}

void func_800e1df8_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    obj->field_08 = 0;
}

void func_800e1e00_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;

    data_800e87cc_slot0f[(s8)obj->field_09](o);
}

void func_800e1e40_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    int i;

    func_801519b4((Object *)&data_800e8508_slot0f);
    for (i = 0; i < 3; i++) {
        Slot0fRec8508 *rec = &data_800e8518_slot0f[i];

        if (obj->field_0a == i) {
            rec->field_0b = 0x10;
        } else {
            rec->field_0b = 0x1a;
        }
        func_801519b4((Object *)rec);
    }
}
