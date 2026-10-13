/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern Object *data_8007ef34_slot2b;

void apply_velocity_and_acceleration_2_slot2b(Object *o);

/*
 * What it does (inferred, not an original name): an update step of an
 * object that falls toward a floor line given by another object (the one
 * data_8007ef34_slot2b points at). When the floor's ground_y is less than
 * the object's pos_y the object has landed: it is set on the floor, its
 * state is reset and a sequence is started. Otherwise advance_sequence_checked runs.
 */
void update_fall_until_ground_then_start_sequence_slot2b(Object *o) {
    s16 y;

    apply_velocity_and_acceleration_2_slot2b(o);
    if (data_8007ef34_slot2b->ground_y < o->pos_y) {
        o->state_1++;
        /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        do {
            y = data_8007ef34_slot2b->ground_y;
            o->data_ptr_a = spawned_object_data_a;
            o->pos_y_frac = 0;
            o->field_0c = 0;
            o->image_row = 0;
            o->field_90 = (void *)0x800fb100;
            o->pos_y = y;
            o->data_ptr_b = spawned_object_data_b;
        } while (0);
        set_sequence_entry(o, data_8017c850);
        play_side_sound_at(o, data_8007ef34_slot2b->side, 0x10);
    } else {
        advance_sequence_checked(o);
    }
}
