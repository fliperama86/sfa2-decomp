/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012f1b8(Object *object) {
    if (object->field_60 == 1 && object->field_6b != 0 && object->frame->field_06 != 0) {
        if (object->field_cd != 0) {
            func_8012d310(object);
        } else {
            func_8012f230(object);
        }
    }
}

void func_8012f230(Object *object) {
    if ((object->field_130 & 0x2000) == 0) {
        func_80130efc(object);
    } else {
        object->field_157 = 0;
        if (object->field_29f != 0) {
            func_8012d264(object);
        } else {
            if ((object->field_130 & 0x4000) == 0) {
                func_801308c4(object, 0xc);
            } else {
                object->field_157 = 1;
                func_801308c4(object, 0xe);
            }
        }
    }
}
