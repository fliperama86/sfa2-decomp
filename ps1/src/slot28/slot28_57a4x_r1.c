/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004ea18_slot28[])(Object *);
extern HudState *data_8018f5a0;

void func_800257a4_slot28(Object *obj) {
    data_8004ea18_slot28[data_8018f5a0->field_52](obj);
    func_80138164();
    func_8011abe4();
}
