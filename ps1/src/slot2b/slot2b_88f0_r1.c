/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;
extern SequenceStep *data_8007eb48_slot2b[];

void func_8007893c_slot2b(Object *o);

void func_800788f0_slot2b(Object *o) {
    o->field_0e = data_8007ef34_slot2b->field_0e;
    func_8007893c_slot2b(o);
}

void func_8007891c_slot2b(Object *o) {
    func_8007893c_slot2b(o);
}

void func_8007893c_slot2b(Object *o) {
    func_80130700(o, data_8007eb48_slot2b[((Slot2bObj *)o)->field_48]);
}

void func_80078978_slot2b(Object *o) {
    o->field_0e = data_8007ef34_slot2b->field_0e;
    o->field_4c = (((Slot2bObj *)data_8007ef34_slot2b->other)->field_10 - ((Slot2bObj *)data_8007ef34_slot2b)->field_10) >> 6;
    o->field_54 = 0;
    o->field_50 = 0x30000;
    o->field_58 = 0xffff6000;
    func_80138070(o, 4);
}

void func_800789e0_slot2b(Object *o) {
    o->field_0e = data_8007ef34_slot2b->field_0e;
    ((Slot2bObj *)o)->field_47 = 0x28;
    func_8007893c_slot2b(o);
}
