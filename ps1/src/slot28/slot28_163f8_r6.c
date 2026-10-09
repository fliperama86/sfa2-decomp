/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80051788_slot28[])(Object *);

void func_80027d54_slot28(Object *obj) {
    data_80051788_slot28[data_8018f5a0->field_50](obj);
}
