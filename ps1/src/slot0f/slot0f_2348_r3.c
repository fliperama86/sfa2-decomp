/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRec8508 data_800e85f8_slot0f;
extern Slot0fRec8508 data_800e8608_slot0f;
extern u8 data_800e8558_slot0f[];
extern u8 data_800e85a8_slot0f[];
extern u8 data_800e8588_slot0f[];
extern Slot0fRec8618 data_800e8618_slot0f[];

void func_800e25e0_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    int i;
    Slot0fRec8618 *r;
    u8 *p;
    if (obj->field_0b == 0) {
        p = (u8 *)&data_800e85f8_slot0f;
    } else {
        p = (u8 *)&data_800e8608_slot0f;
    }
    func_801519b4(p);
    if ((s8)obj->field_00 == 2) {
        p = data_800e8558_slot0f;
    } else {
        p = data_800e85a8_slot0f;
        if ((s8)obj->field_00 == 3) {
            p = data_800e8588_slot0f;
        }
    }
    func_801519b4(p);
    for (i = 0; i < 2; i++) {
        r = &data_800e8618_slot0f[i];
        if (obj->field_0c == i) {
            r->field_0b = 0x10;
        } else {
            r->field_0b = 0x1a;
        }
        func_801519b4(r);
    }
}
