/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd264_slot05_06[];
extern ObjectFn data_801dd26c_slot05_06[];
extern ObjectFn data_801dd278_slot05_06[];
extern ObjectFn data_801dd280_slot05_06[];

void func_801c86b4_slot05_06(Object *obj) {
    data_801dd264_slot05_06[obj->field_07](obj);
}

void func_801c86f4_slot05_06(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801c8724_slot05_06(Object *obj) {
    func_80142a14(obj);
}

void func_801c8744_slot05_06(Object *obj) {
    data_801dd26c_slot05_06[obj->field_12a >> 1](obj);
}

void func_801c8788_slot05_06(Object *obj) {
    data_801dd278_slot05_06[obj->field_07](obj);
}

void func_801c87c8_slot05_06(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801c87f8_slot05_06(Object *obj) {
    func_80142a14(obj);
}

void func_801c8818_slot05_06(Object *obj) {
    data_801dd280_slot05_06[obj->field_07](obj);
}
