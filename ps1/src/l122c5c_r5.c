/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void toggle_pause_flags_on_button(void) {
    unsigned char v;
    if (game_state.field_74 == 0) {
        if (((HudBig *)data_8018f5a0)->step_count == 1 && game_state.field_04 == 0) {
            if ((game_state.mode & 1) && (data_801a696a & 0x800) && game_state.field_31 < 2) {
                if (((HudBig *)data_8018f5a0)->substate_index == 0) {
                    game_state.field_31 = game_state.field_31 ^ 1;
                }
                func_80123f78(0);
            }
            if ((game_state.mode & 2) && (data_801a6976 & 0x800)
                && (game_state.field_31 == 0 || game_state.field_31 == 2)) {
                if (((HudBig *)data_8018f5a0)->substate_index == 0) {
                    game_state.field_31 = game_state.field_31 ^ 2;
                }
                func_80123f78(1);
            }
        } else {
            game_state.field_31 = 0;
        }
    }
}

void func_80123f78(int a) {
    int i;
    u8 *base = data_801ac6a8;
    u8 *p;
    if (game_state.field_31 != 0) {
        game_state.field_09 = 1;
        game_state.palette_swap_flag = 0;
        i = 0;
        stop_all_keys_clear_table();
        set_menu_command_by_code(2, 0);
        p = base;
        scale_palette_rows_and_refresh();
        for (; i < 2; i++) {
            write_player_color_bytes_to_table(p, i);
            func_80133ed0(p, i);
            func_80133fd8(p, i);
            p[(i << 4) + 0x188] = 0;
            p[(i << 4) + 0x189] = 0x49;
            p[(i << 4) + 0x18a] = 0x73;
            p[(i << 4) + 0x1a8] = 0;
            p[(i << 4) + 0x1a9] = 0x49;
            p[(i << 4) + 0x1aa] = 0x73;
        }
    } else {
        game_state.field_09 = 0;
        set_menu_command_by_code(3, 0);
        func_801260ac(0, 0x20, 0);
        func_801260ac(1, 0x20, 0);
        func_801260ac(2, 0x20, 0);
        func_801260ac(3, 0x20, 0);
        func_801260ac(4, 0x20, 0);
        queue_five_jobs();
        ((HudBig *)data_8018f5a0)->substate_index = 0;
        for (i = 0; i < 2; i++) {
            base[(i << 4) + 0x188] = 0x49;
            base[(i << 4) + 0x189] = 0xc9;
            base[(i << 4) + 0x18a] = 0xf3;
            base[(i << 4) + 0x1a8] = 0x49;
            base[(i << 4) + 0x1a9] = 0xc9;
            base[(i << 4) + 0x1aa] = 0xf3;
        }
    }
}
