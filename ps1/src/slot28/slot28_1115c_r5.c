/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80046990_slot28[])(Object *);
extern HudState *data_8018f5a0;
void func_80021d64_slot28(Object *obj, int arg);

void func_80021774_slot28(Object *obj) {
    data_80046990_slot28[data_8018f5a0->field_52](obj);
    func_80138164();
    func_8011abe4();
}

void func_800217cc_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->field_52 = h->field_52 + 1;
    func_80021d64_slot28(obj, 0);
}
