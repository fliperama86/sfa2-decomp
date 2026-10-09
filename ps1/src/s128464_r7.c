/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80129d14(Object *object) {
    int arg = 0;
    object->field_07 = object->field_07 + 1;
    if (object->field_cd != 0) {
        if (object->field_157 != 0) {
            object->field_07 = object->field_07 + 1;
            arg = 1;
        }
    } else {
        if (object->kind == 10) arg = 0x30;
        if (object->field_7e != 0) {
            object->field_278 = 0;
            arg = 2;
        }
    }
    func_80130678(object, arg);
    func_80129da8(object);
}
