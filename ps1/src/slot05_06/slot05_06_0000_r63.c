/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801312b8(Object *object);
void func_801cd718_slot05_06(Object *obj);
void func_801cd838_slot05_06(Object *obj);
extern ObjectFn data_801dd580_slot05_06[];
extern ObjectFn data_801dd58c_slot05_06[];
void func_80130dc0(Object *obj);
void func_80131468(Object *object);
u8 func_80149b80(Object *obj);

void func_801cd698_slot05_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801cd6d8_slot05_06(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801cd838_slot05_06(obj);
    } else {
        func_801cd718_slot05_06(obj);
    }
}

void func_801cd718_slot05_06(Object *obj) {
    data_801dd580_slot05_06[obj->field_12a >> 1](obj);
}

void func_801cd75c_slot05_06(Object *obj) {
    data_801dd58c_slot05_06[obj->field_07](obj);
}

void func_801cd79c_slot05_06(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_801cd7d4_slot05_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    } else {
        func_80131468(obj);
    }
}
