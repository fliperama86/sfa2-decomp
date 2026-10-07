/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c1f58_slot04_07[];

void func_801b22bc_slot04_07(Object *obj) {
    data_801c1f58_slot04_07[obj->field_12a >> 1](obj);
}
