/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007ede8_slot2b[];
extern ObjectFn data_8007edf4_slot2b[];

void set_sequence_from_table_index_entry_slot2b(Object *o);

void call_handler_by_state_1_3_slot2b(Object *o) {
    data_8007ede8_slot2b[o->state_1](o);
}

void func_80078b90_slot2b(Object *o) {
    advance_sequence_checked(o);
}

void advance_state_1_and_call_table_entry_slot2b(Object *o) {
    o->state_1 = o->state_1 + 1;
    set_sequence_from_table_index_entry_slot2b(o);
}

void advance_state_0_on_sequence_loop_flag_slot2b(Object *o) {
    if ((s16)o->step_flags < 0) {
        o->state_0 = o->state_0 + 1;
    }
    advance_sequence_checked(o);
}

void advance_state_0_on_sequence_loop_flag_2_slot2b(Object *o) {
    if ((s16)o->step_flags < 0) {
        o->state_0 = o->state_0 + 1;
    }
    advance_sequence_checked(o);
}

void call_handler_by_state_1_4_slot2b(Object *o) {
    data_8007edf4_slot2b[o->state_1](o);
}
