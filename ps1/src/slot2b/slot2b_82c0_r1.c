/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079a2c_slot2b[];
extern s16 data_8007ef2c_slot2b;
void apply_velocity_and_acceleration_slot2b(Object *o);

void move_until_landing_then_start_sequence_5_slot2b(Object *o) {
    apply_velocity_and_acceleration_slot2b(o);
    if (data_8007ef2c_slot2b >= o->pos_y) {
        advance_sequence_checked(o);
    } else {
        o->state_2++;
        o->pos_y_frac = 0;
        o->vel_y = 0;
        o->accel_y = 0;
        o->pos_y = data_8007ef2c_slot2b;
        set_sequence_by_sound_side(o, 5);
    }
}
