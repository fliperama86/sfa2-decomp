/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80042314_slot28[])(Object *);

void func_80020534_slot28(Object *obj) {
    data_80042314_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
