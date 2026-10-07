/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0750_slot04_09(Object *obj);
void func_801b0984_slot04_09(Object *obj);
void func_801b0790_slot04_09(Object *obj);
void func_801b08f4_slot04_09(Object *obj);
extern ObjectFn data_801c7bb4_slot04_09[];

void func_801b0710_slot04_09(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b0750_slot04_09(obj);
    } else {
        func_801b0984_slot04_09(obj);
    }
}

void func_801b0750_slot04_09(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b0790_slot04_09(obj);
    } else {
        func_801b08f4_slot04_09(obj);
    }
}

void func_801b0790_slot04_09(Object *obj) {
    data_801c7bb4_slot04_09[obj->field_07](obj);
}
