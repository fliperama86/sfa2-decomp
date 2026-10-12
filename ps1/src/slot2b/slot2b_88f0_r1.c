/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;
extern SequenceStep *data_8007eb48_slot2b[];

void set_sequence_from_table_index_entry_slot2b(Object *o);

void func_800788f0_slot2b(Object *o) {
    o->field_0e = data_8007ef34_slot2b->field_0e;
    set_sequence_from_table_index_entry_slot2b(o);
}

void func_8007891c_slot2b(Object *o) {
    set_sequence_from_table_index_entry_slot2b(o);
}

void set_sequence_from_table_index_entry_slot2b(Object *o) {
    set_sequence_entry(o, data_8007eb48_slot2b[((Slot2bObj *)o)->table_index]);
}

void start_hop_toward_other_shift_6_slot2b(Object *o) {
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
    set_sequence_from_table_index_entry_slot2b(o);
}
