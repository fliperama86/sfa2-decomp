/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012c5cc(Object *object) {
    int step;
    u16 index;
    object->field_0b = object->field_158;
    object->field_60 = 0;
    object->field_46 = 0;
    if (object->field_157 != 0) {
        step = object->field_62;
        if (step >= 2) {
            step = 2;
        }
        index = step + 7;
        func_801308c4(object, index);
    } else if (object->field_61 == 8 || object->field_61 == 0) {
        step = object->field_62;
        if (step >= 2) {
            step = 2;
        }
        index = step + 1;
        func_801308c4(object, index);
    } else {
        step = object->field_62;
        if (step >= 2) {
            step = 2;
        }
        index = step + 4;
        func_801308c4(object, index);
    }
    func_80131e88(object);
    if (object->field_6b == 0) {
        func_8012cca8(object);
    }
}
