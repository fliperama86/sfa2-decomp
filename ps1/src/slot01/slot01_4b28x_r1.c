/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot01Rec2b484 data_8002b484_slot01[];
extern SequenceStep *data_8002bb2c_slot01[];
extern Object *data_80033bb0_slot01;

void func_80014b28_slot01(Object *obj, int arg) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    void *t;
    Object *p;
    t = obj->field_3c;
    data_80033bb0_slot01 = t;
    t = (void *)0x800767c0;
    *(u32 *)&obj->field_90 = (u32)t;
    data_80033bb0_slot01->kind = obj->field_48;
    p = data_80033bb0_slot01;
    obj->field_98 = data_8002b484_slot01[p->kind].field_00;
    obj->field_9c = data_8002b484_slot01[p->kind].field_04;
    func_80130768(obj, p->kind, data_8002bb2c_slot01);
}
