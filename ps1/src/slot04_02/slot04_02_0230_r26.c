/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b63b8_slot04_02(Object *obj);
void func_801b65f0_slot04_02(Object *obj);
extern ObjectFn data_801c65a0_slot04_02[];

void func_801b6378_slot04_02(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b65f0_slot04_02(obj);
    } else {
        func_801b63b8_slot04_02(obj);
    }
}

void func_801b63b8_slot04_02(Object *obj) {
    data_801c65a0_slot04_02[obj->field_07](obj);
}
