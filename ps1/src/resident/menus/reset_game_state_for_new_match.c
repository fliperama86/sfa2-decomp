/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void reset_game_state_for_new_match(GameState *g) {
    screen_state->substep_index = 0;
    g->timer_or_bits = 0;
    screen_state->step_index_2 = 0;
    g->countdown_timer_2 = 0;
    screen_state->pass_index = 0;
    g->field_cc = 0;
    g->field_4d = 0;
    g->field_4e = 0;
    g->side_bits_2 = 0;
    g->active_flag = 0;
    g->field_09 = 1;
    g->gate_flag = 1;
    g->field_4f = 1;
    g->field_65 = 0xff;
    player_left.win_count = 0;
    player_right.win_count = 0;
    player_left.meter = 0;
    player_right.meter = 0;
    if (player_left.kind == 0x13) {
        player_left.kind = 0x11;
    }
    if (player_right.kind == 0x13) {
        player_right.kind = 0x11;
    }
    kind_handlers_enabled = 0;
    g->init_timer_value = 0x3c;
    g->round_time = g->table_value_b;
    clear_arrays_and_init_pools_and_sprite_tables();
    call_handler_by_kind_index_a();
    init_hud_primitives();
    spawn_position_table_objects_and_list_objects();
    stop_all_keys_clear_table();
    if (*(u16 *)input_block != 0) {
        data_80197f1c = 0xff;
    }
}
