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
    func_80130700(o, data_8007eb48_slot2b[((Slot2bObj *)o)->table_index]);
}

void func_80078978_slot2b(Object *o) {
    o->field_0e = data_8007ef34_slot2b->field_0e;
    o->vel_x = (((Slot2bObj *)data_8007ef34_slot2b->other)->pos_x_hi - ((Slot2bObj *)data_8007ef34_slot2b)->pos_x_hi) >> 6;
    o->accel_x = 0;
    o->vel_y = 0x30000;
    o->accel_y = 0xffff6000;
    func_80138070(o, 4);
}

void func_800789e0_slot2b(Object *o) {
    o->field_0e = data_8007ef34_slot2b->field_0e;
    ((Slot2bObj *)o)->countdown_timer = 0x28;
    func_8007893c_slot2b(o);
}
