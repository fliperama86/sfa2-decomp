/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5e58_slot04_0f[];

void func_801b4ebc_slot04_0f(Object *obj);

void func_801b4ebc_slot04_0f(Object *obj) {
    data_801c5e58_slot04_0f[obj->field_05](obj);
}
