/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c0614_slot04_0a[];
extern ObjectFn data_801c0634_slot04_0a[];

void func_801b18e4_slot04_0a(Object *obj) {
    data_801c0614_slot04_0a[obj->field_07](obj);
}

void func_801b1924_slot04_0a(Object *obj) {
    data_801c0634_slot04_0a[obj->field_07](obj);
}
