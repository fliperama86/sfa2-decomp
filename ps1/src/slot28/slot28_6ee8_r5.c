/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_80017944_slot28(Object *obj, int arg);
extern Object *data_8005193c_slot28[];

void func_800174fc_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->field_52 = h->field_52 + 1;
    func_80017944_slot28(obj, 0);
}

void func_80017530_slot28(Object *obj) {
    if (*(s16 *)&data_8005193c_slot28[3]->field_3a < 0) {
        HudState *h = data_8018f5a0;
        h->field_52 = h->field_52 + 1;
        func_801282d4();
    }
}
