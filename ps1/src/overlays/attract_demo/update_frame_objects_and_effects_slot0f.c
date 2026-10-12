/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_800e84f0_slot0f[];

void update_frame_objects_and_effects_slot0f(GameState *g, int x) {
    g->frame_counter++;
    g->field_a8 = x;
    update_player_inputs_and_pick_handler();
    log_both_players_state();
    g->field_65 = player_left.field_165 | player_right.field_165;
    update_units();
    separate_overlapping_players();
    update_blocks();
    call_active_block_handlers();
    if (data_80190568 != 0) {
        set_capped_value_and_ot_indices(g);
    }
    if (g->state_param != 0) {
        g->state_param--;
    }
    check_collisions_unless_paused();
    update_hud_bars_and_meters();
    select_buffers_and_run_first_setup();
}

void queue_item_from_table_by_index_slot0f(int i) {
    add_item_to_table(data_800e84f0_slot0f[i]);
}
