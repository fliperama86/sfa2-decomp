/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142a14(Object *object);
extern ObjectFn data_801c24d4_slot04_0b[];
extern ObjectFn data_801c24e0_slot04_0b[];

void func_801b08d4_slot04_0b(Object *obj) {
    func_80142a14(obj);
}

void func_801b08f4_slot04_0b(Object *obj) {
    data_801c24d4_slot04_0b[obj->field_12a >> 1](obj);
}

void func_801b0938_slot04_0b(Object *obj) {
    data_801c24e0_slot04_0b[obj->field_07](obj);
}
