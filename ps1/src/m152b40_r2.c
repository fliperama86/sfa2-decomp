/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80152f80(GameState *state) {
    if (*(u32 *)0x1f80000c == 0) {
        if (*(u32 *)0x1f800010 != 0) {
            u32 v = table_801802dc[*(u32 *)0x1f800014];
            u32 lo;
            data_801ac624 = v;
            lo = v & 0xffff;
            state->kind_index = v >> 16;
            data_801a8067 = data_801a83fb = lo;
            func_8014e890(v >> 16);
            func_8014eb9c(lo, lo);
        } else {
            func_80119144(2, 7);
            func_801192bc(1);
        }
    } else {
        func_80119144(2, 7);
        do {
            func_801192bc(1);
        } while (!(state->wait_flag & 0x80));
    }
    close_vab_and_sep(2);
    func_8014f3b8(4, 0);
}

void func_80153088(void) {
    data_8018d210[0].kind = 0;
    data_8018d210[0].side = 0;
    data_8018d210[0].active_flag = 0;
    data_8018d210[0].state_index = 0;
    data_8018d210[0].field_04 = 0;
    data_8018d210[0].substep_index = 0;
    data_8018d210[0].current_count = 0;
    data_8018d210[0].table_index = 0;
    data_8018d210[0].field_09 = 0;
    data_8018d210[0].field_0b = 0;
    data_8018d210[0].countdown_timer = 0;
    data_8018d210[0].field_10 = 0;
    data_8018d210[1].kind = 0;
    data_8018d210[1].side = 1;
    data_8018d210[1].active_flag = 0;
    data_8018d210[1].state_index = 0;
    data_8018d210[1].field_04 = 0;
    data_8018d210[1].substep_index = 0;
    data_8018d210[1].current_count = 0;
    data_8018d210[1].table_index = 0;
    data_8018d210[1].field_09 = 0;
    data_8018d210[1].field_0b = 0;
    data_8018d210[1].countdown_timer = 0;
    data_8018d210[1].field_10 = 0;
}
