/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8002c728_slot28[])(Object *);

void func_80014950_slot28(Object *obj) {
}

void func_80014958_slot28(Object *obj) {
    data_8002c728_slot28[data_8018f5a0->field_50](obj);
}
