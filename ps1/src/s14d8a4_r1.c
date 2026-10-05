/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014d8a4(Object *object) {
    if (object->field_06 == 0) func_8014d99c(object);
}

void func_8014d8d4(Object *object) {
    if (object->field_209 == 0) {
        func_8014d8a4(object);
    } else if (object->field_209 == 1) {
        func_8014d91c(object);
    }
}

void func_8014d91c(Object *object) {
    if (object->field_06 == 0 || object->field_21b == 0) func_8014d99c(object);
}

void func_8014d95c(Object *object) {
    if (*(u16 *)&object->field_04 == 1 && object->field_06 == 0) func_8014d99c(object);
}

void func_8014d99c(Object *object) {
    object->field_207 = 0;
    object->field_208 = 0;
    object->field_209 = 0;
}

void func_8014d9ac(Object *object) {
    u16 **table;
    u16 *p;
    u16 first;
    object->field_24f = 0;
    object->field_253 = 0;
    if (object->side == 0) table = scr_c4_left;
    else table = scr_174_right;
    p = table[object->field_20b >> 1];
    data_80189460 = p + 1;
    first = *p;
    object->field_228 = (s32)data_80189460;
    object->field_23c = (s32)data_80189460;
    object->field_224 = 0;
    object->field_25d = 0;
    object->field_20c = first;
}
