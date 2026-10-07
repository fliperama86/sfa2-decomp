/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7ce4_slot04_09[];
extern ObjectFn data_801c7cf0_slot04_09[];

void func_801b2e74_slot04_09(Object *obj) {
    data_801c7ce4_slot04_09[obj->field_12a >> 1](obj);
}

void func_801b2eb8_slot04_09(Object *obj) {
    data_801c7cf0_slot04_09[obj->field_07](obj);
}
