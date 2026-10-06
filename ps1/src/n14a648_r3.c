/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014bfa8(Object *object) {
    if ((s16) object->other->field_5c < *(s16 *) data_80189460++) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}
