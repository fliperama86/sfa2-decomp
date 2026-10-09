/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c05a4_slot04_0a[];
extern ObjectFn data_801c05d0_slot04_0a[];

void func_801b1690_slot04_0a(Object *obj) {
    data_801c05a4_slot04_0a[obj->field_15a](obj);
}

void func_801b16d0_slot04_0a(Object *obj) {
    data_801c05d0_slot04_0a[obj->field_15a](obj);
}
