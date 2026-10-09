/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051c90_slot28[];
extern ObjectRef data_80051c94_slot28;
void func_80025ebc_slot28(void);
void func_80025e2c_slot28(Object *obj, int arg);

void func_80025880_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_52++;
        func_80025ebc_slot28();
        data_80051c90_slot28[0]->field_01 = 0;
        data_80051c94_slot28.p->field_01 = 0;
        func_80025e2c_slot28(obj, 1);
        func_80128370();
    }
}
