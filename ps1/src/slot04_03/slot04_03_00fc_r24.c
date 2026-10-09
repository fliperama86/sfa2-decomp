/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b40f0_slot04_03(Object *obj);
void func_801b46a4_slot04_03(Object *obj);
void func_801b4130_slot04_03(Object *obj);
void func_801b42bc_slot04_03(Object *obj);
extern ObjectFn data_801c09e8_slot04_03[];

void func_801b4090_slot04_03(Object *obj, Object *unused) {
    func_8011f38c(obj);
}

void func_801b40b0_slot04_03(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b40f0_slot04_03(obj);
    } else {
        func_801b46a4_slot04_03(obj);
    }
}

void func_801b40f0_slot04_03(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b4130_slot04_03(obj);
    } else {
        func_801b42bc_slot04_03(obj);
    }
}

void func_801b4130_slot04_03(Object *obj) {
    data_801c09e8_slot04_03[obj->field_07](obj);
}
