/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern SequenceStep *data_8002904c_slot27[];
extern ObjectFn data_80028e84_slot27[];
void func_80016de4_slot27(Object *obj);

void func_80016bd8_slot27(Object *unused) {
    int t = (*(Object **)&data_80190458)->side * 2;
    data_8019045c[0] = t;
    data_8019045c[0] = ref_other.p->field_03 + t;
}

void func_80016c10_slot27(Object *obj) {
    func_80016de4_slot27(obj);
    data_8019045c[0] = 0;
}
