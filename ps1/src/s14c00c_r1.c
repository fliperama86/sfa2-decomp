/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014c00c(Object *object) {
    if ((s16)object->other->field_5c < (s16)object->field_5c) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014c058(Object *object) {
    Object *other = object->other;
    if (other->field_45 != 1 || other->field_50 >= 0) {
        func_8014c528(object);
    } else {
        func_8014c914(object);
    }
}

void func_8014c0b0(Object *object) {
    Object *other = object->other;
    if (other->field_45 == 1 && other->field_50 < 0) {
        func_8014c528(object);
    } else {
        func_8014c914(object);
    }
}
