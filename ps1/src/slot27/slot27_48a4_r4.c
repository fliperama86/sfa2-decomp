/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot27Rec7d14 data_80027d14_slot27[];
extern SequenceStep *data_80027fa8_slot27[];
extern ObjectFn data_80028110_slot27[];
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];

void func_80014c1c_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    o->field_90 = (void *)0x80038000;
    o->field_98 = data_80017c28_slot27;
    o->field_9c = data_8001aa14_slot27;
    o->field_01 = 0;
    o->field_7a = 0;
    o->field_7c = 0x1e0;
    o->field_0d = 0;
    o->field_0b = 0;
    o->field_3a = 0;
    o->field_09 = data_80027d14_slot27[o->field_03].field_00;
    obj->field_12 = data_80027d14_slot27[o->field_03].field_02;
    *(u16 *)&o->pos_y = data_80027d14_slot27[o->field_03].field_04;
}

void func_80014cd8_slot27(Object *obj) {
    func_80130768(obj, obj->field_03, data_80027fa8_slot27);
}

void func_80014d04_slot27(Object *obj) {
    ref_other.p = obj->field_3c;
    (*(Object **)&data_80190458) = obj->other;
    data_80028110_slot27[obj->field_04](obj);
}
