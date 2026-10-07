/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5598_slot04_06[];
extern ObjectFn data_801c55a4_slot04_06[];
void func_80130dc0(Object *obj);

void func_801b583c_slot04_06(Object *obj) {
    data_801c5598_slot04_06[obj->field_12a >> 1](obj);
}

void func_801b5880_slot04_06(Object *obj) {
    data_801c55a4_slot04_06[obj->field_07](obj);
}

void func_801b58c0_slot04_06(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}
