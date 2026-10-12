/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014c00c(Object *object) {
    if ((s16)object->other->health < (s16)object->health) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014c058(Object *object) {
    Object *other = object->other;
    if (other->active_state != 1 || other->vel_y >= 0) {
        func_8014c528(object);
    } else {
        func_8014c914(object);
    }
}

void func_8014c0b0(Object *object) {
    Object *other = object->other;
    if (other->active_state == 1 && other->vel_y < 0) {
        func_8014c528(object);
    } else {
        func_8014c914(object);
    }
}
