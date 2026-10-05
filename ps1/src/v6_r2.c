/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012cf1c(Object *object) {
    int index;
    if (func_8012eff8(object) != 0) {
        object->field_07 = object->field_07 + 1;
        func_8013786c(object);
        object->field_258 = 0;
        object->field_259 = 0;
        object->field_25a = 0;
    } else {
        func_801376b8(object);
    }
    if ((s16)object->field_5c < 0) {
        if ((s16)object->field_46 >= 8) {
            object->field_07 = 3;
            func_801308c4(object, 0x1b);
        }
    } else if (object->field_61 == 8 && object->field_cd != 0) {
        func_8012d2ac(object);
    } else if (object->field_61 != 8 || (object->field_130 & 0x2000) == 0) {
        func_80130efc(object);
    } else {
        index = 0xc;
        object->field_60 = 1;
        object->field_157 = 0;
        if ((object->field_130 & 0x4000) != 0) {
            object->field_157 = 1;
            index = 0xe;
        }
        func_801308c4(object, index);
    }
}
