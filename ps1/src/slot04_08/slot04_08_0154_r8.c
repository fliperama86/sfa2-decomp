/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3b98_slot04_08[];
extern ObjectFn data_801c3bc0_slot04_08[];

void func_801b1364_slot04_08(Object *obj) {
    data_801c3b98_slot04_08[obj->field_15a](obj);
}

void func_801b13a4_slot04_08(Object *obj) {
    data_801c3bc0_slot04_08[obj->field_15a](obj);
}
