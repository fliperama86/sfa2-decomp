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
            state->field_40 = v >> 16;
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
        } while (!(state->field_f0 & 0x80));
    }
    func_801203b4(2);
    func_8014f3b8(4, 0);
}

void func_80153088(void) {
    data_8018d210[0].kind = 0;
    data_8018d210[0].side = 0;
    data_8018d210[0].field_02 = 0;
    data_8018d210[0].field_03 = 0;
    data_8018d210[0].field_04 = 0;
    data_8018d210[0].field_05 = 0;
    data_8018d210[0].field_06 = 0;
    data_8018d210[0].field_08 = 0;
    data_8018d210[0].field_09 = 0;
    data_8018d210[0].field_0b = 0;
    data_8018d210[0].field_0c = 0;
    data_8018d210[0].field_10 = 0;
    data_8018d210[1].kind = 0;
    data_8018d210[1].side = 1;
    data_8018d210[1].field_02 = 0;
    data_8018d210[1].field_03 = 0;
    data_8018d210[1].field_04 = 0;
    data_8018d210[1].field_05 = 0;
    data_8018d210[1].field_06 = 0;
    data_8018d210[1].field_08 = 0;
    data_8018d210[1].field_09 = 0;
    data_8018d210[1].field_0b = 0;
    data_8018d210[1].field_0c = 0;
    data_8018d210[1].field_10 = 0;
}
