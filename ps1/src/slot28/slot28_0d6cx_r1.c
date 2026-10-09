/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051b18_slot28[];
extern ObjectRef data_80051b50_slot28;

void func_80020d6c_slot28(Object *obj) {
    Object *p;
    p = data_80051b50_slot28.p;
    if (((Slot28Obj *)p)->field_3a < 0) {
        data_8018f5a0->field_60 = 0x3c;
        p = data_80051b18_slot28[0];
        p->field_48 = 0xff;
        data_8018f5a0->field_52++;
    }
}
