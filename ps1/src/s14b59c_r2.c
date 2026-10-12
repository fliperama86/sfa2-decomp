/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014bcd8(Object *object) {
    if (func_80149d48(object)) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014bd24(Object *object) {
    Object *other = object->other;
    if (other->active_state == 1 && other->variant_index == 1) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014bd7c(Object *object) {
    Object *other = object->other;
    if (other->active_state == 1 && other->variant_index == 0) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014bdd4(Object *object) {
    Object *other = object->other;
    if (other->active_state == 1 && other->variant_index == 0xff) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014be2c(Object *object) {
    if ((u8)func_8014e758(object)) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}
