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
    if (object->script_arg == 0) {
        func_8014c5f8(object);
    } else if (object->script_arg == 2) {
        if (func_8014a170(object, (u32 *)table_8017d798) != 0) {
            func_8014c5f8(object);
        } else {
            func_8014c914(object);
        }
    }
}

void func_8014c5f8(Object *object) {
    object->script_mode = 1;
    object->saved_script_word = object->field_20c;
    object->script_pointer = (s32)data_80189460;
    func_8014da18(object);
    func_8014c914(object);
}
