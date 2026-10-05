/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern Hud *data_8018f5a0;

void func_80120c14(GameState *g) {
    unsigned char i;
    unsigned n;
    unsigned w;
    int mask;
    g->field_74 = 0;
    if (g->field_58 != 0) {
        g->field_74 = 0xff;
        g->field_58 = 0;
    } else if (g->field_6d != 0) {
        i = 0;
        w = table_8016e720[g->field_56];
        n = g->field_33 & 0xf;
        mask = 1;
        for (i = 0; i < n; i++) {
            mask <<= 1;
        }
        if (mask & w) {
            g->field_58 = -1;
        }
    }
}

void func_80120ca0(void) {
    unsigned v = data_8018f5a0->field_48;
    if (v == 0) {
        func_80120cf0();
    } else if (v == 1) {
        func_80120f98();
    }
}
