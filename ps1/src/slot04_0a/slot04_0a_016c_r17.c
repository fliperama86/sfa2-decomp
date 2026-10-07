/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c06b0_slot04_0a[];
extern ObjectFn data_801c06c4_slot04_0a[];

void func_801b2d24_slot04_0a(Object *obj) {
    data_801c06b0_slot04_0a[obj->field_07](obj);
}

void func_801b2d64_slot04_0a(Object *obj) {
    data_801c06c4_slot04_0a[obj->field_07](obj);
}
