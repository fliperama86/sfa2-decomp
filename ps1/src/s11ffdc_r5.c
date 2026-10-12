/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80120c14(GameState *g) {
    unsigned char i;
    unsigned n;
    unsigned w;
    int mask;
    g->field_74 = 0;
    if (g->pending_flag != 0) {
        g->field_74 = 0xff;
        g->pending_flag = 0;
    } else if (g->field_6d != 0) {
        i = 0;
        w = table_8016e720[g->table_index_3];
        n = g->loop_counter & 0xf;
        mask = 1;
        for (i = 0; i < n; i++) {
            mask <<= 1;
        }
        if (mask & w) {
            g->pending_flag = -1;
        }
    }
}

void func_80120ca0(GameState *state) {
    unsigned v = ((Hud *)data_8018f5a0)->stage;
    if (v == 0) {
        func_80120cf0(state);
    } else if (v == 1) {
        func_80120f98(state);
    }
}
