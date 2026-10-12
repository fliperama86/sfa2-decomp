/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80055f40_slot01;
void queue_score_text_and_win_strips_slot01(void);
void loop_updates_while_wait_flag_slot01(void);
void start_wait_with_two_halfword_stores_slot01(void);
void spawn_object_and_loop_while_busy_slot01(void);

void finish_slot01_step_or_reset_hud_slot01(void) {
    Rect rect;
    int t;

    queue_score_text_and_win_strips_slot01();
    if (game_state.field_04 != 0) {
        return;
    }
    if (is_pad_pressed_for_side_bits_or_mode() == 0) {
        t = game_state.timer_or_bits - 1;
        game_state.timer_or_bits = t;
        if ((s16)t >= 0) {
            return;
        }
    }
    if (game_state.active_flag != 0) {
        set_menu_command_by_code(6, 2);
        rect.x = 0x70;
        rect.y = 0x1e0;
        rect.w = 0x10;
        rect.h = 0x20;
        func_80158028(&rect, data_801a37e4);
        func_80157d9c(0);
        loop_updates_while_wait_flag_slot01();
        data_80055f40_slot01->active_count = 0;
        start_wait_with_two_halfword_stores_slot01();
        spawn_object_and_loop_while_busy_slot01();
        loop_updates_while_wait_flag_slot01();
        data_8018f5a0->phase_index = 3;
        data_8018f5a0->step_index = 0;
        game_state.countdown_2 = 0;
        data_8018f5a0->substep_index = 0;
        game_state.timer_or_bits = 0;
        data_8018f5a0->step_index_2 = 0;
        game_state.countdown_timer_2 = 0;
        data_8018f5a0->pass_index = 0;
        game_state.field_cc = 0;
        data_8018f5a0->pass_index = 0;
        game_state.status_flags = 0;
        game_state.field_80 = 0;
        game_state.field_09 = 1;
    } else {
        data_8018f5a0->substep_index++;
        game_state.field_09 = 1;
    }
}
