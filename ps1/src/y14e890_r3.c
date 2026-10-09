/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80155a24(void) {
    GameState *g;
    func_801519b4(&data_801812f4);
    g = &game_state;
    if ((game_state.field_31 == 1 && (data_801a696a & 0x4000)) ||
        (game_state.field_31 == 2 && (data_801a6976 & 0x4000))) {
        table_801811ed[data_8018d264 * 11] = 0x1a;
        data_8018d264 = (data_8018d264 + 1) & 1;
        table_801811ed[data_8018d264 * 11] = 0x14;
        func_80120554(0, 0, 0x34d);
    }
    if ((g->field_31 == 1 && (data_801a696a & 0x1000)) ||
        (g->field_31 == 2 && (data_801a6976 & 0x1000))) {
        table_801811ed[data_8018d264 * 11] = 0x1a;
        data_8018d264 = (data_8018d264 + 255) & 1;
        table_801811ed[data_8018d264 * 11] = 0x14;
        func_80120554(0, 0, 0x34d);
    }
    if ((g->field_31 == 1 && (data_801a696a & 0x20)) ||
        (g->field_31 == 2 && (data_801a6976 & 0x20))) {
        func_80120554(0, 0, 0x34c);
        if (data_8018d264 != 0) {
            data_8018f5a0->field_52 = 0;
        } else {
            g->field_17 = 0;
            g->field_2f = 0;
            g->field_30 = 0;
            g->field_31 = 0;
            *(u32 *)0x1f80000c = 1;
            func_80152ee8();
        }
    }
}
