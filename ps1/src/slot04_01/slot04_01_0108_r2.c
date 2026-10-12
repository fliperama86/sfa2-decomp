/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void restore_facing_and_set_sequence_0_slot04_01(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void advance_state_2_when_timers_clear_slot04_01(Object *obj) {
    if (game_state.field_64 == 0 && game_state.countdown_timer == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}
