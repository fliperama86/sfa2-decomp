/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800518fc_slot28[];

void func_800168a8_slot28(Object *obj) {
    if (*(s16 *)&data_800518fc_slot28[3]->field_3a < 0) {
        HudState *h = data_8018f5a0;
        h->field_52 = h->field_52 + 1;
        func_801282d4();
    }
}
