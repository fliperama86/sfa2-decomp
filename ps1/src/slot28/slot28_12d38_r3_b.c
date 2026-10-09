/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004aa10_slot28[])(Object *);

void func_800230c8_slot28(Object *obj) {
    data_8004aa10_slot28[data_8018f5a0->field_50](obj);
}
