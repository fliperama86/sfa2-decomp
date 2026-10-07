/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c24a8_slot04_0b[];

void func_801b05bc_slot04_0b(Object *obj) {
}

void func_801b05c4_slot04_0b(Object *obj) {
}

void func_801b05cc_slot04_0b(Object *obj) {
    data_801c24a8_slot04_0b[obj->field_06](obj);
}
