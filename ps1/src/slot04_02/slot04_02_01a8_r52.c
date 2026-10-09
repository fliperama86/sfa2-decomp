/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6594_slot04_02[];

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->field_04 = a;
    obj->field_05 = b;
    obj->field_06 = c;
    obj->field_07 = d;
}

void func_801b6334_slot04_02(Object *obj) {
    data_801c6594_slot04_02[obj->field_128 >> 1](obj);
}
