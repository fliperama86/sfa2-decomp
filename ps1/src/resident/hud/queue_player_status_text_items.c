/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void queue_player_status_text_items(void) {
    GameState *g = &game_state;
    Object *o;
    u8 s;

    if (data_8019032d == 0) {
        if (data_80190137 == 0 && data_80190138 == 0) {
            o = &player_left;
            if (data_80198069 != 4) {
                s = data_801980b0 & 0xf;
                if (s >= 12) {
                    add_item_to_table(table_80172318[s]);
                    if (s == 14) {
                        queue_digit_text(g, o);
                    }
                }
            } else {
                add_rank_item_for_phase_2_or_3(g, o);
                queue_name_text(g, o);
            }
            o = &player_right;
            if (data_801983fd != 4) {
                s = data_80198444 & 0xf;
                if (s >= 12) {
                    add_item_to_table(table_80172328[s]);
                    if (s == 14) {
                        queue_digit_text(&game_state, o);
                    }
                }
            } else {
                add_rank_item_for_phase_2_or_3(g, o);
                queue_name_text(g, o);
            }
        } else if (data_80190138 != 0) {
            if (input_block[1] != 0) {
                if (g->loop_counter & 0x20) {
                    if (g->mode & 1) {
                        add_item_to_table(&data_80172380[0]);
                    } else if (g->mode & 2) {
                        add_item_to_table(&data_80172380[1]);
                    }
                }
            } else if (g->versus_flag != 0 && input_block[0] != 0) {
                if (g->loop_counter & 0x20) {
                    if (g->mode & 1) {
                        add_item_to_table(&data_80172380[2]);
                    } else if (g->mode & 2) {
                        add_item_to_table(&data_80172380[3]);
                    }
                }
            }
        }
    }
}
