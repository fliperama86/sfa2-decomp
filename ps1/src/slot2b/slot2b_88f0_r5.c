/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;
extern ObjectFn data_8007ee00_slot2b[];

void apply_velocity_and_acceleration_2_slot2b(Object *o);
void spawn_child_object_above_self_slot2b(Object *o);
void spawn_child_object_at_random_offset_slot2b(Object *o);
void set_sequence_from_table_index_entry_slot2b(Object *o);

void func_80078ddc_slot2b(Object *o) {
    apply_velocity_and_acceleration_2_slot2b(o);
    if (data_8007ef34_slot2b->ground_y < o->pos_y) {
        o->state_0 = o->state_0 + 1;
        o->pos_y = data_8007ef34_slot2b->ground_y;
        o->pos_y_frac = 0;
        spawn_child_object_above_self_slot2b(o);
        spawn_child_object_at_random_offset_slot2b(o);
        spawn_child_object_at_random_offset_slot2b(o);
    } else {
        advance_sequence_checked(o);
    }
}

void call_handler_by_state_1_5_slot2b(Object *o) {
    data_8007ee00_slot2b[o->state_1](o);
}

void func_80078eb0_slot2b(Object *o) {
    if ((s16)data_8007ef34_slot2b->step_flags >= 0) {
        advance_sequence_checked(o);
    } else {
        o->state_1 = o->state_1 + 1;
        o->variant_index = o->variant_index + 2;
        set_sequence_from_table_index_entry_slot2b(o);
    }
}

void func_80078f0c_slot2b(Object *o) {
    advance_sequence_checked(o);
}
