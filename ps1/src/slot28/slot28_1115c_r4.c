/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80046988_slot28[])(Object *);

void func_8002146c_slot28(Object *obj) {
}

void func_80021474_slot28(Object *obj) {
    data_80046988_slot28[data_8018f5a0->field_50](obj);
}
