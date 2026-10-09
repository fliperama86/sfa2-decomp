/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800445b4_slot28[])(Object *);

void func_8002068c_slot28(Object *obj) {
    data_800445b4_slot28[data_8018f5a0->field_50](obj);
}
