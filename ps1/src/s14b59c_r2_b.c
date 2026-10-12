/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014bf58(Object *object) {
    if (object->link_flags == 0 && object->linked_object_3 == 0) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}
