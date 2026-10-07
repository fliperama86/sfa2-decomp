/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_800289d4_slot28[])(Object *);

void func_80013854_slot28(Object *obj);

void func_80013834_slot28(Object *obj) {
    func_80013854_slot28(obj);
}

void func_80013854_slot28(Object *obj) {
    data_800289d4_slot28[data_8018f5a0->field_4e](obj);
}
