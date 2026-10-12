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
    o->variant_index = a;
    func_80130700(o, data_8007eb48_slot2b[a]);
}

void func_800787c8_slot2b(Object *o) {
    data_8007ef34_slot2b = o->parent;
    data_8007ed98_slot2b[o->state_0](o);
}

void func_80078818_slot2b(Object *o) {
    Object *p;

    o->state_0++;
    p = data_8007ef34_slot2b;
    o->field_1c = p->field_1c;
    o->field_0c = p->field_0c;
    o->image_row = data_8007ef34_slot2b->image_row;
    o->facing_side = data_8007ef34_slot2b->facing_side;
    data_8007eda8_slot2b[o->table_index](o);
}

void func_800788b8_slot2b(Object *o) {
    o->spawn_flag = 1;
    o->field_0e = data_8007ef34_slot2b->field_0e;
    func_8007893c_slot2b(o);
}
