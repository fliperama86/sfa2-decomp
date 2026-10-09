/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001e8bc_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->field_60 - 1;
    h->field_60 = t;
    if ((s16)t == 0) {
        h->field_52 = h->field_52 + 1;
        func_801282d4();
    }
}
