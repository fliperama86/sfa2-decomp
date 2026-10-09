/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b09c4_slot04_09(Object *obj);
void func_801b0a54_slot04_09(Object *obj);
extern ObjectFn data_801c7bc4_slot04_09[];

void func_801b0964_slot04_09(Object *obj) {
    func_80142a14(obj);
}

void func_801b0984_slot04_09(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b0a54_slot04_09(obj);
    } else {
        func_801b09c4_slot04_09(obj);
    }
}

void func_801b09c4_slot04_09(Object *obj) {
    data_801c7bc4_slot04_09[obj->field_07](obj);
}
