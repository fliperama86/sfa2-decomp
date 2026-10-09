/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c55c4_slot04_06[])(Object *);

void func_801b5db0_slot04_06(Object *obj) {
    data_801c55c4_slot04_06[obj->field_04](obj);
}
