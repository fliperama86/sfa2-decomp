/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_game_frame(GameState *g) {
    int x;
    if (((g->versus_flag == 0 && g->field_226 == 0) ? g->leader_bits : (g->leader_bits || g->field_226)) == 0) {
        toggle_pause_flags_on_button();
    }
    if (g->field_31 != 0) {
        if (g->field_74 == 0) {
            call_substate_table_entry_2();
        }
    } else if (g->versus_flag != 0 && data_80197f1c != 0) {
        if (g->field_74 == 0) {
            call_substate_table_entry();
        }
        update_blocks();
        call_active_block_handlers();
        set_capped_value_and_ot_indices(g);
    } else {
        table_8016e7ac[data_8018f5a0->substep_index](g);
        if (g->field_4f != 0) {
            return;
        }
        x = 0;
        if (g->countdown_timer != 0) {
            g->countdown_timer_b--;
            x = 1;
            if (g->countdown_timer_b == 0) {
                x = 0;
                g->countdown_timer--;
                g->countdown_timer_b = g->reload_value;
            }
        }
        g->frame_counter++;
        g->field_a8 = x;
        if ((g->field_84 & 0x80) == 0) {
            update_script_indices_and_counters_unless_mode3(g);
            update_player_inputs_and_pick_handler();
            g->field_65 = player_left.field_165 | player_right.field_165;
            update_units();
            separate_overlapping_players();
            log_both_players_state();
            push_player_out_of_type_9_object_boxes();
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
        } else if (g->field_84 == 0x81) {
            g->field_84 = 0;
        }
    }
    select_buffers_and_run_first_setup();
}
