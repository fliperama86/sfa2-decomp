/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003f98c_slot28[])(Object *);
void func_8001ee74_slot28(Object *obj, int arg);

void func_8001e7d0_slot28(Object *obj) {
    data_8003f98c_slot28[data_8018f5a0->pass_index](obj);
    func_80138164();
    func_8011abe4();
}

void func_8001e828_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->pass_index = h->pass_index + 1;
    func_8001ee74_slot28(obj, 0);
}
