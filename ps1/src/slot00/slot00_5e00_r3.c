/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079150_slot00[];
extern ObjectFn data_80079158_slot00[];
void func_800760e8_slot00(Object *obj);
void func_800761c8_slot00(Object *obj);
u8 func_80149b80(Object *obj);

void func_800760a8_slot00(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 == 0) {
        func_800760e8_slot00(obj);
    } else {
        func_800761c8_slot00(obj);
    }
}

void func_800760e8_slot00(Object *obj) {
    data_80079150_slot00[obj->field_07](obj);
}

void func_80076128_slot00(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_80076160_slot00(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_800761c8_slot00(Object *obj) {
    data_80079158_slot00[obj->field_12a >> 1](obj);
}
