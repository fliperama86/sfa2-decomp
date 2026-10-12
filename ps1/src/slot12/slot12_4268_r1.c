/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_phase_two_clear_step_slot12(Object *obj) {
    data_8018f5a0->phase_index = 2;
    data_8018f5a0->step_index = 0;
}

void noop_slot00_4_slot12(Object *obj) {
}

void advance_hud_phase_index_and_reset_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    data_8018f5a0->phase_index++;
    if (game_state.mode_flag_2 != 0) {
        data_8018f5a0->phase_index = 6;
    }
    data_8018f5a0->step_index = 0;
    data_8018f5a0->substep_index = 0;
    data_8018f5a0->step_index_2 = 0;
    data_8018f5a0->pass_index = 0;
    obj->field_ee = 0;
    obj->field_f0 = 0;
    clear_players_arrays_state_and_init_pools();
    clear_arrays_and_init_pools_and_sprite_tables();
}
