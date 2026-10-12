/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int move_by_velocity_return_y_minus_ground_slot04_06(Object *obj);

void move_advance_state_0_at_ground_timed_limit_check_slot04_06(Object *obj) {
    GameState *g = &game_state;

    if ((move_by_velocity_return_y_minus_ground_slot04_06(obj) << 16) >= 0) {
        obj->state_0++;
    }
    if ((((Slot04aObj *)obj)->timer & 0x80) == 0) {
        ((Slot04aObj *)obj)->timer -= 1;
        if ((((Slot04aObj *)obj)->timer & 0x80) == 0) {
            clear_or_set_flag_by_pos_x_limit(obj);
            return;
        }
    }
    if (g->frame_counter & 1) {
        clear_or_set_flag_by_pos_x_limit(obj);
    }
}
