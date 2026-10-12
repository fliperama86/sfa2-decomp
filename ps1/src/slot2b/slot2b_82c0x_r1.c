/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079a2c_slot2b[];
extern s16 data_8007ef2c_slot2b;
void apply_velocity_and_acceleration_slot2b(Object *o);

void call_handler_by_state_2_unless_paused_slot2b(Object *o) {
    if ((game_state_second.pause_flag_a | game_state_second.pause_flag_b) != 0) {
        clear_or_set_flag_by_pos_x_limit(o);
        return;
    }
    data_80079a2c_slot2b[o->state_2](o);
    set_state_0_if_x_outside_margin_narrow(o);
    clear_or_set_flag_by_pos_x_limit(o);
}
