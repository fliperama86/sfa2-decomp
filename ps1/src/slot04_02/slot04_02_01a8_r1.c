/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6198_slot04_02[];
extern ObjectFn data_801c61a4_slot04_02[];

void func_801b01a8_slot04_02(Object *obj) {
    data_801c6198_slot04_02[obj->input_dir_0 >> 1](obj);
}

void func_801b01ec_slot04_02(Object *obj) {
    data_801c61a4_slot04_02[obj->input_dir_second >> 1](obj);
}
