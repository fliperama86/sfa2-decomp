/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c1574_slot04_05[];
extern ObjectFn data_801c1580_slot04_05[];

void func_801b2960_slot04_05(Object *obj) {
    data_801c1574_slot04_05[obj->field_12a >> 1](obj);
}

void func_801b29a4_slot04_05(Object *obj) {
    data_801c1580_slot04_05[obj->field_07](obj);
}
