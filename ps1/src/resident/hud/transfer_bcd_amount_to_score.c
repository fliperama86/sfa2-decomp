/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void transfer_bcd_amount_to_score(GameState *g) {
    Object *o = g->focus_object;
    if (o->mode_flag_2 == 0) {
        set_selected_value_by_input_threshold((Entity *)g);
        if ((u8)is_pad_pressed_for_leader_bits_or_mode() != 0 || g->bcd_step_amount > g->bcd_amount) {
            screen_state->step_index_2++;
            g->countdown_timer_2 = 0x5a;
            o->score = add_bcd(o->score, g->bcd_amount << 8);
            g->bcd_amount = 0;
            call_handler_by_data_index();
            play_sound_at(0, 0, 0x34d);
            call_menu_command_if_input_enabled(g);
        } else {
            g->bcd_amount = subtract_bcd(g->bcd_amount, g->bcd_step_amount);
            o->score = add_bcd(o->score, g->bcd_step_amount << 8);
            call_handler_by_data_index();
            if ((game_state.frame_counter & 3) == 0) {
                play_sound_at(0, 0, 0x34d);
            }
        }
    }
}
