/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004c6f8_slot28[])(Object *);

void func_80024784_slot28(Object *obj) {
    data_8004c6f8_slot28[data_8018f5a0->field_50](obj);
}
