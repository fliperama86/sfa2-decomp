/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b05d4_slot04_05(Object *obj);
void func_801b0938_slot04_05(Object *obj);
void func_801b0614_slot04_05(Object *obj);
void func_801b0840_slot04_05(Object *obj);
extern ObjectFn data_801c1374_slot04_05[];

void func_801b0594_slot04_05(Object *obj) {
    if (obj->field_128 != 0) {
        func_801b0938_slot04_05(obj);
    } else {
        func_801b05d4_slot04_05(obj);
    }
}

void func_801b05d4_slot04_05(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b0840_slot04_05(obj);
    } else {
        func_801b0614_slot04_05(obj);
    }
}

void func_801b0614_slot04_05(Object *obj) {
    data_801c1374_slot04_05[obj->field_07](obj);
}
