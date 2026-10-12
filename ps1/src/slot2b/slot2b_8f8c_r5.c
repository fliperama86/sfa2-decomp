/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;

void apply_velocity_and_acceleration_2_slot2b(Object *obj);

void advance_sequence_and_state_on_flag_slot2b(Object *o) {
    if ((s16)o->step_flags < 0) {
        o->state_0++;
    }
    advance_sequence_checked(o);
}
