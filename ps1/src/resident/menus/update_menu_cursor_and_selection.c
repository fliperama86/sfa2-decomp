/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

#define PRESSED(m) ((g->field_31 == 1 && (data_801a696a & (m))) || \
                    (g->field_31 == 2 && (data_801a6976 & (m))))

void update_menu_cursor_and_selection(void) {
    GameState *g = &game_state;
    if (g->versus_flag == 0) {
        add_item_to_table(&data_801812e4);
        if (PRESSED(0x20)) {
            if (data_8018d260 == 0) {
                data_8018f5a0->pass_index = 0;
            } else if (data_8018d260 == 1) {
                data_8018f5a0->pass_index = 3;
            } else if (data_8018d260 == 2) {
                data_8018f5a0->pass_index = 4;
            }
            play_sound_at(0, 0, 0x34c);
        }
        if (PRESSED(0x4000)) {
            data_801811aa[data_8018d260 * 16] = 0x1a;
            data_8018d260 = data_8018d260 + 1;
            if (data_8018d260 >= 3) {
                data_8018d260 = 0;
            }
            data_801811aa[data_8018d260 * 16] = 0x14;
            play_sound_at(0, 0, 0x34d);
        }
        if (PRESSED(0x1000)) {
            data_801811aa[data_8018d260 * 16] = 0x1a;
            data_8018d260 = data_8018d260 + 255;
            if (data_8018d260 & 0x80) {
                data_8018d260 = 2;
            }
            data_801811aa[data_8018d260 * 16] = 0x14;
            play_sound_at(0, 0, 0x34d);
        }
    } else if (*(u16 *)data_801a6984 == 0) {
        add_item_to_table(&data_80181304);
        if (PRESSED(0x40)) {
            clear_game_state_flag_and_call_palette_setup();
            play_sound_at(0, 0, 0x34c);
        } else if (PRESSED(0x20)) {
            if (data_8018d260 == 0) {
                clear_game_state_flag_and_call_palette_setup();
            } else if (data_8018d260 == 1) {
                data_8018f5a0->pass_index = 3;
            } else if (data_8018d260 == 2) {
                clear_game_state_flag_and_call_palette_setup();
                g->field_65 = 0xff;
                data_80197f1c = 0xff;
                reset_game_state_for_new_match(&game_state);
            } else if (data_8018d260 == 3) {
                data_8018f5a0->pass_index = 4;
            }
            play_sound_at(0, 0, 0x34c);
        } else if (PRESSED(0x4000)) {
            data_80181206[data_8018d260 * 16] = 0x1a;
            data_8018d260 = data_8018d260 + 1;
            if (data_8018d260 >= 4) {
                data_8018d260 = 0;
            }
            data_80181206[data_8018d260 * 16] = 0x14;
            play_sound_at(0, 0, 0x34d);
        } else if (PRESSED(0x1000)) {
            data_80181206[data_8018d260 * 16] = 0x1a;
            data_8018d260 = data_8018d260 + 255;
            if (data_8018d260 & 0x80) {
                data_8018d260 = 3;
            }
            data_80181206[data_8018d260 * 16] = 0x14;
            play_sound_at(0, 0, 0x34d);
        }
    } else {
        add_item_to_table(&data_80181314);
        if (PRESSED(0x20)) {
            if (data_8018d260 == 0) {
                clear_game_state_flag_and_call_palette_setup();
                g->field_65 = 0xff;
                data_80197f1c = 0xff;
                reset_game_state_for_new_match(&game_state);
            } else if (data_8018d260 == 1) {
                data_8018f5a0->pass_index = 4;
            }
            play_sound_at(0, 0, 0x34c);
        } else if (PRESSED(0x5000)) {
            data_8018124a[data_8018d260 * 9] = 0x1a;
            data_8018d260 = (data_8018d260 + 1) & 1;
            data_8018124a[data_8018d260 * 9] = 0x14;
            play_sound_at(0, 0, 0x34d);
        }
    }
}
