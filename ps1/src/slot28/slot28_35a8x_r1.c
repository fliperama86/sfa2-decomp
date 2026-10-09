/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051be4_slot28[];
extern ObjectRef data_80051c1c_slot28;
void func_80023be0_slot28(Object *obj, int arg);

void func_800235a8_slot28(Object *obj) {
    Object *p;
    p = data_80051c1c_slot28.p;
    if (((Slot28Obj *)p)->field_3a < 0) {
        data_8018f5a0->field_60 = 0x12c;
        data_8018f5a0->field_52++;
        p = data_80051be4_slot28[3];
        p->field_48 = 0xff;
        func_80023be0_slot28(obj, 1);
    }
}
