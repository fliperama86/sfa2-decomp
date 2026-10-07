/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot01Rec31b98 data_80031b98_slot01[];

void func_800135cc_slot01(Object *obj) {
    Cell20 *c = data_80031b98_slot01[data_801a27d0].cells;
    PrimTag *ot = (PrimTag *)data_801987c8 + 0x1d;
    int i;
    int j;
    int t = obj->field_5c;

    obj->field_5c = t - 2;
    if ((s16)obj->field_5c < ((Slot01Obj *)obj)->field_5e) {
        obj->field_5c = t + 0x1e;
    }
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 3; j++) {
            c->field_0a = obj->field_5c + (i << 5);
            ((PrimTag *)c)->addr = ot->addr;
            ot->addr = (u32)c;
            c++;
        }
    }
    ((PrimTag *)c)->addr = ot->addr;
    ot->addr = (u32)c;
}

void func_800136c0_slot01(Object *obj) {
    obj->field_04 = obj->field_04 + 1;
}
