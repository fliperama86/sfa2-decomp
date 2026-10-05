/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u16 func_80130470(Object *object);
u8 func_80130258(Object *object);
u8 func_8012f800(Object *object);
void func_80130678(Object *object, int index);

void func_8012ba80(Object *object) {
    if (object->field_cd != 0) {
        if ((s16)object->field_3a < 0) {
            func_80131468(object);
        } else {
            object->field_0b = object->field_158;
            func_80130efc(object);
        }
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if (func_80130258(object)) {
        func_80130280(object);
    } else if (func_8012f970(object)) {
        object->field_07 = 2;
        object->field_157 = 1;
        object->field_0b = object->field_158;
        func_80130678(object, 0x19);
    } else if (func_8012f800(object)) {
        func_8012f838(object);
    } else if ((s16)object->field_3a < 0) {
        func_80131468(object);
    } else {
        object->field_0b = object->field_158;
        func_80130efc(object);
    }
}
