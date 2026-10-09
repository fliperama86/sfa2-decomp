/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051c90_slot28[];
extern HudState *data_8018f5a0;
void func_801282d4(void);

void func_80025be8_slot28(Object *obj) {
    if (*(s16 *)&data_80051c90_slot28[3]->field_3a < 0) {
        HudState *h = data_8018f5a0;
        h->field_52 = h->field_52 + 1;
        func_801282d4();
    }
}
