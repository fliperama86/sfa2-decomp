/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800373f4_slot28[])(Object *);

void func_80019aa0_slot28(Object *obj) {
    data_800373f4_slot28[data_8018f5a0->field_50](obj);
}
