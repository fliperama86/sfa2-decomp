/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_8014e40c(Object *object);

void func_8014c468(Object *object) {
    if (object->field_67 != 0) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}
