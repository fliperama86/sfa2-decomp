/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef28_slot2b;
void apply_velocity_and_acceleration_slot2b(Object *o);
void spawn_child_object_above_self_slot2b(Object *o);
void spawn_child_object_at_random_offset_slot2b(Object *o);
void set_state_0_to_2_and_clear_substates_slot2b(Object *o);

void move_then_advance_or_spawn_three_children_at_end_slot2b(Object *o) {
    apply_velocity_and_acceleration_slot2b(o);
    if ((s16)o->step_flags >= 0) {
        advance_sequence_checked(o);
    } else {
        o->state_2++;
        spawn_child_object_above_self_slot2b(o);
        spawn_child_object_at_random_offset_slot2b(o);
        spawn_child_object_at_random_offset_slot2b(o);
        set_sequence_by_sound_side(o, (o->vel_table_index >> 1) + 0xc);
    }
}

void advance_sequence_or_set_state_0_to_2_slot2b(Object *o) {
    if ((s16)o->step_flags >= 0) {
        advance_sequence_checked(o);
    } else {
        set_state_0_to_2_and_clear_substates_slot2b(o);
    }
}

void set_state_0_to_2_and_clear_substates_slot2b(Object *o) {
    o->active_count = 2;
    o->state_0 = 2;
    o->state_1 = 0;
    o->state_2 = 0;
    o->state_3 = 0;
}

void set_state_0_to_2_and_clear_substates_2_slot2b(Object *o) {
    o->active_count = 2;
    o->state_0 = 2;
    o->state_1 = 0;
    o->state_2 = 0;
    o->state_3 = 0;
}

void set_state_0_to_2_and_clear_substates_3_slot2b(Object *o) {
    o->active_count = 2;
    o->state_0 = 2;
    o->state_1 = 0;
    o->state_2 = 0;
    o->state_3 = 0;
}

void set_state_0_to_2_and_clear_substates_4_slot2b(Object *o) {
    o->active_count = 2;
    o->state_0 = 2;
    o->state_1 = 0;
    o->state_2 = 0;
    o->state_3 = 0;
}

void set_state_0_to_3_and_clear_linked_object_slot2b(Object *o) {
    o->state_0 = 3;
    o->state_1 = 0;
    o->state_2 = 0;
    o->state_3 = 0;
    o->active_count = 2;
    data_8007ef28_slot2b->linked_object_3 = 0;
}
