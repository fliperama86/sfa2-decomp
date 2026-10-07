/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c06a8_slot04_03[];
extern ObjectFn data_801c06d8_slot04_03[];

void func_801b22ac_slot04_03(Object *obj) {
    data_801c06a8_slot04_03[obj->field_07](obj);
}

void func_801b22ec_slot04_03(Object *obj) {
    data_801c06d8_slot04_03[obj->field_07](obj);
}
