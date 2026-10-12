/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80121d80(Object *unused) {
    HudState *h = data_8018f5a0;
    h->countdown_timer = (s16)h->countdown_timer - 1;
    if ((s16)h->countdown_timer < 0) {
        h->pass_index = 0;
        h->sub_timer = 4;
        h->step_index_2++;
    }
}
