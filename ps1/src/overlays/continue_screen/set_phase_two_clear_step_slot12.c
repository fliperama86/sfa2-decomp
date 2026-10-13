/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void set_phase_two_clear_step_slot12(Object *obj) {
    screen_state->phase_index = 2;
    screen_state->step_index = 0;
}

void noop_slot00_4_slot12(Object *obj) {
}

void advance_hud_phase_index_and_reset_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    screen_state->phase_index++;
    if (game_state.mode_flag_2 != 0) {
        screen_state->phase_index = 6;
    }
    screen_state->step_index = 0;
    screen_state->substep_index = 0;
    screen_state->step_index_2 = 0;
    screen_state->pass_index = 0;
    obj->field_ee = 0;
    obj->field_f0 = 0;
    clear_players_arrays_state_and_init_pools();
    clear_arrays_and_init_pools_and_sprite_tables();
}
