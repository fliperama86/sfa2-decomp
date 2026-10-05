/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012d170(Object *object) {
    int index;
    if (func_8012eff8(object) != 0) {
        object->field_07 = object->field_07 + 1;
        object->field_0b = (object->field_72 ^ 1) & 1;
        func_8012d394(object);
    } else if ((s16)object->field_5c < 0) {
        func_8012d264(object);
    } else if (object->field_cd != 0) {
        func_8012d2ac(object);
    } else {
        index = 0xc;
        if ((object->field_130 & 0x2000) == 0) {
            func_80130efc(object);
        } else {
            object->field_157 = 0;
            if (object->field_29f != 0) {
                func_8012d264(object);
            } else {
                if ((object->field_130 & 0x4000) != 0) {
                    object->field_157 = 1;
                    index = 0xe;
                }
                func_801308c4(object, index);
            }
        }
    }
}
