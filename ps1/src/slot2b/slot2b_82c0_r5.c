/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8007eb48_slot2b[];
extern ObjectFn data_8007ed98_slot2b[];
extern ObjectFn data_8007eda8_slot2b[];
extern Object *data_8007ef34_slot2b;
void func_8007893c_slot2b(Object *o);

void func_8007878c_slot2b(Object *o, u8 a) {
    o->field_48 = a;
    func_80130700(o, data_8007eb48_slot2b[a]);
}

void func_800787c8_slot2b(Object *o) {
    data_8007ef34_slot2b = o->field_3c;
    data_8007ed98_slot2b[o->field_04](o);
}

void func_80078818_slot2b(Object *o) {
    Object *p;

    o->field_04++;
    p = data_8007ef34_slot2b;
    o->field_1c = p->field_1c;
    o->field_0c = p->field_0c;
    o->field_0d = data_8007ef34_slot2b->field_0d;
    o->field_0b = data_8007ef34_slot2b->field_0b;
    data_8007eda8_slot2b[o->field_03](o);
}

void func_800788b8_slot2b(Object *o) {
    o->field_44 = 1;
    o->field_0e = data_8007ef34_slot2b->field_0e;
    func_8007893c_slot2b(o);
}
