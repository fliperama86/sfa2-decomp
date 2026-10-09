/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6420_slot04_02[];
extern ObjectFn data_801c646c_slot04_02[];

void func_801b4434_slot04_02(Object *obj) {
    data_801c6420_slot04_02[obj->field_07](obj);
}

void func_801b4474_slot04_02(Object *obj) {
    data_801c646c_slot04_02[obj->field_07](obj);
}
