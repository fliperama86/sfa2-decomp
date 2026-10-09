/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051b04_slot28[];

void func_8001fe78_slot28(Object *obj) {
    if (*(s16 *)&data_80051b04_slot28[3]->field_3a < 0) {
        HudState *h = data_8018f5a0;
        h->field_52 = h->field_52 + 1;
        func_801284f0();
    }
}
