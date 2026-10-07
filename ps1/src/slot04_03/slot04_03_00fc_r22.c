/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c09bc_slot04_03[])(Object *);

void func_801b3c48_slot04_03(Object *obj, Object *unused) {
    data_801c09bc_slot04_03[obj->field_05](obj);
}
