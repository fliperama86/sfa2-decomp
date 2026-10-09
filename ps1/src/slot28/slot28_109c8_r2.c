/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern SequenceStep *data_8004384c_slot28[];
extern ObjectRef data_80051b40_slot28;
extern Object *data_80051b44_slot28[];
extern ObjectRef data_80051b48_slot28;
void func_800210f0_slot28(void);
void func_80021060_slot28(Object *obj, int arg);

void func_80020b4c_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_52++;
        data_80051b44_slot28[0]->field_01 = 0;
        data_80051b48_slot28.p->field_01 = 0;
        func_800210f0_slot28();
        func_80130768(data_80051b40_slot28.p, 0, data_8004384c_slot28);
        func_80021060_slot28(obj, 1);
        func_80128370();
    }
}
