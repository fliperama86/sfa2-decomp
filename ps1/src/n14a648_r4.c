/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014c3ac(Object *object) {
    s16 v = *data_80189460++;
    if (object->other->kind == v) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014c40c(Object *object) {
    s16 v = *data_80189460++;
    if (object->kind == v) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}
