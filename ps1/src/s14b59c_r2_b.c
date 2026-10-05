/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_8014e758(Object *object);

void func_8014bf58(Object *object) {
    if (object->field_240 == 0 && object->field_14c == 0) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}
