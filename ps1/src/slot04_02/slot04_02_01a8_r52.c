/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6594_slot04_02[];

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->state_0 = a;
    obj->state_1 = b;
    obj->state_2 = c;
    obj->state_3 = d;
}

void func_801b6334_slot04_02(Object *obj) {
    data_801c6594_slot04_02[obj->input_dir_0 >> 1](obj);
}
