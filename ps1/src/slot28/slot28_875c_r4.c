/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_8005198c_slot28;
void func_80019090_slot28(Object *obj);

void func_80018dec_slot28(Object *obj) {
    func_80019090_slot28(obj);
    if ((s16)data_8005198c_slot28.p->field_3a < 0) {
        data_8018f5a0->field_52 += 1;
        func_8014f4d4(1, 0x602);
        func_801282d4();
    }
}
