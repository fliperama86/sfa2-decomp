/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800287a8_slot12[])(Object *);

void func_80016328_slot12(Object *obj) {
    data_800287a8_slot12[obj->field_04](obj);
}
