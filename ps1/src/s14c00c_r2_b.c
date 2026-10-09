/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014c528(Object *object) {
    data_8018946c = object->field_224;
    while (func_8014e40c(object) != 0) {
    }
    func_8014c914(object);
}

void func_8014c578(Object *object) {
    if (object->field_20f == 0) {
        func_8014c5f8(object);
    } else if (object->field_20f == 2) {
        if (func_8014a170(object, (u32 *)table_8017d798) != 0) {
            func_8014c5f8(object);
        } else {
            func_8014c914(object);
        }
    }
}

void func_8014c5f8(Object *object) {
    object->field_20a = 1;
    object->field_222 = object->field_20c;
    object->field_228 = (s32)data_80189460;
    func_8014da18(object);
    func_8014c914(object);
}
