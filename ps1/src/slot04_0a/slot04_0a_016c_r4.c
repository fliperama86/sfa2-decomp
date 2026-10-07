/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142a14(Object *object);
extern ObjectFn data_801c0550_slot04_0a[];

void func_801b07ac_slot04_0a(Object *obj) {
    func_80142a14(obj);
}

void func_801b07cc_slot04_0a(Object *obj) {
    data_801c0550_slot04_0a[obj->field_07](obj);
}
