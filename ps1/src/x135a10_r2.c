/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern s8 data_801a6984[];

void func_80135c88(void) {
    GameState *g = &game_state;
    Object *o;
    u8 s;

    if (data_8019032d == 0) {
        if (data_80190137 == 0 && data_80190138 == 0) {
            o = &player_left;
            if (data_80198069 != 4) {
                s = data_801980b0 & 0xf;
                if (s >= 12) {
                    func_801519b4(table_80172318[s]);
                    if (s == 14) {
                        func_80126efc(g, o);
                    }
                }
            } else {
                func_80156c80(g, o);
                func_80156bc8(g, o);
            }
            o = &player_right;
            if (data_801983fd != 4) {
                s = data_80198444 & 0xf;
                if (s >= 12) {
                    func_801519b4(table_80172328[s]);
                    if (s == 14) {
                        func_80126efc(&game_state, o);
                    }
                }
            } else {
                func_80156c80(g, o);
                func_80156bc8(g, o);
            }
        } else if (data_80190138 != 0) {
            if (data_801a6984[1] != 0) {
                if (g->field_33 & 0x20) {
                    if (g->mode & 1) {
                        func_801519b4(&data_80172380[0]);
                    } else if (g->mode & 2) {
                        func_801519b4(&data_80172380[1]);
                    }
                }
            } else if (g->field_30 != 0 && data_801a6984[0] != 0) {
                if (g->field_33 & 0x20) {
                    if (g->mode & 1) {
                        func_801519b4(&data_80172380[2]);
                    } else if (g->mode & 2) {
                        func_801519b4(&data_80172380[3]);
                    }
                }
            }
        }
    }
}
