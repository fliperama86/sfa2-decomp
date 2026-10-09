/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801312b8(Object *object);
void func_801b571c_slot04_06(Object *obj);
void func_801b583c_slot04_06(Object *obj);
extern ObjectFn data_801c5584_slot04_06[];
extern ObjectFn data_801c5590_slot04_06[];
void func_80130dc0(Object *obj);

void func_801b569c_slot04_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b56dc_slot04_06(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b583c_slot04_06(obj);
    } else {
        func_801b571c_slot04_06(obj);
    }
}

void func_801b571c_slot04_06(Object *obj) {
    data_801c5584_slot04_06[obj->field_12a >> 1](obj);
}

void func_801b5760_slot04_06(Object *obj) {
    data_801c5590_slot04_06[obj->field_07](obj);
}

void func_801b57a0_slot04_06(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}
