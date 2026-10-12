/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_8002317c_slot12[];

void func_80013a3c_slot12(Object *obj) {
    int a;
    int b;
    int i;
    u16 *src;
    u16 *dst;
    if (game_state.mode_flag_2 == 0) {
        b = table_8016e5c4[game_state.left_rank_index].frame_param;
        a = table_8016e5c4[game_state.left_rank_index].level;
    } else {
        b = table_8016e614[game_state.right_rank_index].frame_param;
        a = table_8016e614[game_state.right_rank_index].level;
    }
    src = &data_8002317c_slot12[a * 96 + b * 16];
    dst = data_801a3de4;
    for (i = 0; i < 16; i++) {
        *dst++ = *src++;
    }
    src = &data_8002317c_slot12[a * 96 + b * 16];
    dst = data_801a29e4;
    for (i = 0; i < 16; i++) {
        *dst++ = *src++;
    }
}
