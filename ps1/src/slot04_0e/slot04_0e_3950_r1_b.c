/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c60b4_slot04_0e[];

void func_801b430c_slot04_0e(Object *obj) {
    data_801c60b4_slot04_0e[obj->field_12a >> 1](obj);
}
