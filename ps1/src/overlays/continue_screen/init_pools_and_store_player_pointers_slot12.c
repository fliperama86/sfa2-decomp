/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_8002d56c_slot12;
extern Object *data_8002d568_slot12;

void init_pools_and_store_player_pointers_slot12(void) {
    HudState *h = screen_state;
    int one = 1;
    kind_handlers_enabled = one;
    h->substep_index++;
    game_state.field_09 = one;
    game_state.state_2c_flag = 0xff;
    game_state.field_65 = one;
    init_sprite_tables();
    init_object_pools();
    data_8002d56c_slot12 = game_state.object;
    data_8002d568_slot12 = &player_left + (game_state.object->side ^ 1);
}

void increment_substep_index_slot12(void) {
    screen_state->substep_index++;
}

void advance_step_index_and_clear_timers_slot12(void) {
    HudState *h = screen_state;
    h->step_index++;
    h->substep_index = 0;
    game_state.timer_or_bits = 0;
    h->step_index_2 = 0;
    game_state.countdown_timer_2 = 0;
    game_state.field_65 = 0;
    clear_arrays_and_init_pools_and_sprite_tables();
}
