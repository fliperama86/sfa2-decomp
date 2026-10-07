/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142a14(Object *object);
void func_801b0978_slot04_05(Object *obj);
void func_801b0a08_slot04_05(Object *obj);
extern ObjectFn data_801c1388_slot04_05[];

void func_801b0918_slot04_05(Object *obj) {
    func_80142a14(obj);
}

void func_801b0938_slot04_05(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b0a08_slot04_05(obj);
    } else {
        func_801b0978_slot04_05(obj);
    }
}

void func_801b0978_slot04_05(Object *obj) {
    data_801c1388_slot04_05[obj->field_07](obj);
}
