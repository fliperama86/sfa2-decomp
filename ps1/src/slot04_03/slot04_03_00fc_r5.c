/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c0560_slot04_03[])(Object *);

void func_801b0894_slot04_03(Object *obj) {
    obj->field_157 = 1;
    data_801c0560_slot04_03[obj->field_07](obj);
}
