/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80153854(Effect *effect) {
    int t;
    if (((Actor *)effect)->target->round_count >= 2) {
        func_80153514((Actor *)effect);
    } else {
        func_80153d7c((Actor *)effect);
        if (((Actor *)effect)->owner->mode_flag_2 == 0) {
            func_80153dfc((Actor *)effect);
        }
        t = ((Actor *)effect)->countdown_timer - 1;
        ((Actor *)effect)->countdown_timer = t;
        if (t & 0x8000) {
            ((Actor *)effect)->field_02 = 0;
            ((Actor *)effect)->field_05 = 0;
        }
    }
}

void func_801538f0(Actor *a) {
    a->field_02 = 1;
    if (a->owner->effect_code_bc != 0) {
        func_80153a38(a);
    } else {
        func_8015393c(a);
    }
}
