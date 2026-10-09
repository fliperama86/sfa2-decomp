/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7d74_slot04_09[];
extern ObjectFn data_801c7d80_slot04_09[];

void func_801b3e68_slot04_09(Object *obj) {
    data_801c7d74_slot04_09[obj->field_12a >> 1](obj);
}

void func_801b3eac_slot04_09(Object *obj) {
    data_801c7d80_slot04_09[obj->field_07](obj);
}
