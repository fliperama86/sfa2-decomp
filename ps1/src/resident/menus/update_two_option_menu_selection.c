/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void update_two_option_menu_selection(void) {
    GameState *g;
    add_item_to_table(&data_801812f4);
    g = &game_state;
    if ((game_state.field_31 == 1 && (pad_1_pressed & 0x4000)) ||
        (game_state.field_31 == 2 && (pad_2_pressed & 0x4000))) {
        table_801811ed[data_8018d264 * 11] = 0x1a;
        data_8018d264 = (data_8018d264 + 1) & 1;
        table_801811ed[data_8018d264 * 11] = 0x14;
        play_sound_at(0, 0, 0x34d);
    }
    if ((g->field_31 == 1 && (pad_1_pressed & 0x1000)) ||
        (g->field_31 == 2 && (pad_2_pressed & 0x1000))) {
        table_801811ed[data_8018d264 * 11] = 0x1a;
        data_8018d264 = (data_8018d264 + 255) & 1;
        table_801811ed[data_8018d264 * 11] = 0x14;
        play_sound_at(0, 0, 0x34d);
    }
    if ((g->field_31 == 1 && (pad_1_pressed & 0x20)) ||
        (g->field_31 == 2 && (pad_2_pressed & 0x20))) {
        play_sound_at(0, 0, 0x34c);
        if (data_8018d264 != 0) {
            screen_state->pass_index = 0;
        } else {
            g->side_bits = 0;
            g->mode3_active_flag = 0;
            g->versus_flag = 0;
            g->field_31 = 0;
            *(u32 *)0x1f80000c = 1;
            switch_to_next_table_entry_and_wait();
        }
    }
}
