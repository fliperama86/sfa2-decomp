/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012d394(Object *object) {
    if ((s16)object->field_5c < 0) {
        func_8012f4dc(object);
    } else {
        object->state_0 = 1;
        object->state_1 = 1;
        object->state_2 = 2;
        object->state_3 = 0;
        object->field_15b = 0;
        object->field_247 = 0;
    }
}

void func_8012d3e8(Object *object) {
    table_80171a3c[object->state_3](object);
}

void func_8012d428(Object *object) {
    object->state_3 = object->state_3 + 1;
    object->field_159 = 0;
    object->field_157 = 0;
    func_8012ee7c(object);
    object->field_45 = 0xff;
    func_801376b8(object);
    if (object->field_243 != 0) {
        func_8012c494(object);
        if (object->field_15b == 0) {
            object->field_63 = 6;
            func_8012ee7c(object);
        } else {
            func_80141f28(object, 3);
            func_8012d0e0(object);
        }
    } else {
        func_8012d0e0(object);
    }
    if (object->field_163 != 0) {
        func_80148b60(object);
    }
    func_8012d4e4(object);
}
