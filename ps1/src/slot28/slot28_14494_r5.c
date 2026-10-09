/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004c700_slot28[])(Object *);
extern HudState *data_8018f5a0;
void func_800250e4_slot28(Object *obj, int arg);
extern Object *data_80051c50_slot28[];

void func_800249b0_slot28(Object *obj) {
    data_8004c700_slot28[data_8018f5a0->field_52](obj);
    func_80138164();
    func_8011abe4();
}

void func_80024a08_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->field_52 = h->field_52 + 1;
    func_800250e4_slot28(obj, 0);
}

void func_80024a3c_slot28(Object *obj) {
    if (*(s16 *)&data_80051c50_slot28[3]->field_3a < 0) {
        HudState *h = data_8018f5a0;
        h->field_52 = h->field_52 + 1;
        func_801282d4();
    }
}
