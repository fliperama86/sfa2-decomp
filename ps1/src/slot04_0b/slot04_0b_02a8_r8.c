/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142a14(Object *object);
extern void (*data_801c24f4_slot04_0b[])(Object *);

void func_801b0ab4_slot04_0b(Object *obj) {
    func_80142a14(obj);
}

void func_801b0ad4_slot04_0b(Object *obj) {
    obj->field_157 = 1;
    data_801c24f4_slot04_0b[obj->field_07](obj);
}
