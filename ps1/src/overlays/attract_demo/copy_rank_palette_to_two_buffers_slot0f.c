/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 data_800ea9b0_slot0f[];

void copy_rank_palette_to_two_buffers_slot0f(Object *obj) {
    int a;
    int b;
    int i;
    u16 *src;
    u16 *dst;
    if (game_state.mode_flag_2 == 0) {
        b = score_rank_table[game_state.left_rank_index].frame_param;
        a = score_rank_table[game_state.left_rank_index].level;
    } else {
        b = streak_rank_table[game_state.right_rank_index].frame_param;
        a = streak_rank_table[game_state.right_rank_index].level;
    }
    src = &data_800ea9b0_slot0f[a * 96 + b * 16];
    dst = data_801a3de4;
    for (i = 0; i < 16; i++) {
        *dst++ = *src++;
    }
    src = &data_800ea9b0_slot0f[a * 96 + b * 16];
    dst = data_801a29e4;
    for (i = 0; i < 16; i++) {
        *dst++ = *src++;
    }
}
