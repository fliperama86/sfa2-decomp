/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012c7b4(Object *object) {
    int index;
    if (object->field_29f != 0) {
        func_8012c92c(object);
        return;
    }
    object->field_0b = object->field_158;
    if (object->field_45 != 0) {
        object->field_60 = 3;
        object->field_45 = 0xff;
        func_801308c4(object, 0x12);
        if (object->field_6b == 0) {
            func_8012cca8(object);
        }
        return;
    }
    object->field_60 = 1;
    object->field_46 = 0;
    if (object->field_cd != 0) {
        func_8012c89c(object);
        return;
    }
    index = 0xe;
    if (object->field_29f != 0) {
        object->field_157 = 0;
    }
    if (object->field_157 == 0) {
        index = 0xc;
    }
    func_801308c4(object, index);
}
