/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003c28c_slot28[])(Object *);

void func_8001cb34_slot28(Object *obj) {
    data_8003c28c_slot28[data_8018f5a0->field_50](obj);
}
