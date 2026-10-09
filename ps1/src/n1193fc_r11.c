/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;

void func_80121d80(Object *unused) {
    HudState *h = data_8018f5a0;
    h->field_60 = (s16)h->field_60 - 1;
    if ((s16)h->field_60 < 0) {
        h->field_52 = 0;
        h->field_62 = 4;
        h->field_50++;
    }
}
