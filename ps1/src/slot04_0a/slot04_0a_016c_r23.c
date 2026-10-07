/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3a50_slot04_0a(Object *obj);
void func_801b3c34_slot04_0a(Object *obj);
extern ObjectFn data_801c0714_slot04_0a[];

void func_801b3a10_slot04_0a(Object *obj) {
    if (obj->field_128 != 0) {
        func_801b3c34_slot04_0a(obj);
    } else {
        func_801b3a50_slot04_0a(obj);
    }
}

void func_801b3a50_slot04_0a(Object *obj) {
    data_801c0714_slot04_0a[obj->field_07](obj);
}
