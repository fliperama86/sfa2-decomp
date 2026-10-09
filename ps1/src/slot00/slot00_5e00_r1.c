/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079140_slot00[];
void func_80075e50_slot00(Object *obj);
void func_80075e90_slot00(Object *obj);
void func_80075fc8_slot00(Object *obj);
void func_800760a8_slot00(Object *obj);
u8 func_8013f8c4(Object *obj, int a, int b);

void func_80075e10_slot00(Object *obj) {
    if (obj->field_128 == 0) {
        func_80075e50_slot00(obj);
    } else {
        func_800760a8_slot00(obj);
    }
}

void func_80075e50_slot00(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_80075e90_slot00(obj);
    } else {
        func_80075fc8_slot00(obj);
    }
}

void func_80075e90_slot00(Object *obj) {
    data_80079140_slot00[obj->field_07](obj);
}

void func_80075ed0_slot00(Object *obj) {
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0 && func_8013f8c4(obj, -0x11, 0x14) != 0) {
        obj->field_04 = 1;
        obj->field_05 = 2;
        obj->field_06 = 0;
        obj->field_07 = 0;
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}
