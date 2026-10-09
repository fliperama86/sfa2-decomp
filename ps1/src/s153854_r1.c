/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80153854(Effect *effect) {
    int t;
    if (((Actor *)effect)->field_1c->field_6a >= 2) {
        func_80153514((Actor *)effect);
    } else {
        func_80153d7c((Actor *)effect);
        if (((Actor *)effect)->field_18->field_cd == 0) {
            func_80153dfc((Actor *)effect);
        }
        t = ((Actor *)effect)->field_0c - 1;
        ((Actor *)effect)->field_0c = t;
        if (t & 0x8000) {
            ((Actor *)effect)->field_02 = 0;
            ((Actor *)effect)->field_05 = 0;
        }
    }
}

void func_801538f0(Actor *a) {
    a->field_02 = 1;
    if (a->field_18->field_bc != 0) {
        func_80153a38(a);
    } else {
        func_8015393c(a);
    }
}
