/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void darken_backup_rows_and_queue_jobs_slot27(void);
void save_player_select_values_slot27(void);

void advance_hud_step_2_and_load_image_row_slot27(void) {
    Rect rect;
    HudState *h = data_8018f5a0;
    h->step_index_2++;
    rect.x = 0x40;
    rect.y = 0x1e0;
    rect.w = 0x10;
    game_state.field_80 = 0;
    rect.h = 1;
    func_80157fc4(&rect, (u8 *)(0x800ed820 + ((game_state.loop_counter & 3) << 5)));
}

void wait_countdown_2_then_start_next_step_slot27(void) {
    u16 *p = &game_state.countdown_2;
    int t = *p - 1;
    *p = t;
    if ((s16)t == 0) {
        Block172 *b;
        data_8018f5a0->step_index_2++;
        set_menu_command_by_code(6, 2);
        darken_backup_rows_and_queue_jobs_slot27();
        b = pop_block_from_pool();
        if (b != 0) {
            b->active_flag = 1;
            b->handler_index = 0x10;
            b->block_mode = 5;
        }
        game_state.busy_flag = 0xff;
        game_state.side_mask = 0;
        game_state.alt_flag = 0;
        game_state.state_2c_flag = 1;
        save_player_select_values_slot27();
    }
}
