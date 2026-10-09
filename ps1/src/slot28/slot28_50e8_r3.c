/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;

void func_80015310_slot28(Object *obj) {
    HudState *h;
    int t;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        t = h->field_60 - 1;
        h->field_60 = t;
        if ((s16)t == 0) {
            h->field_52 = h->field_52 + 1;
            func_801282d4();
        }
    }
}
