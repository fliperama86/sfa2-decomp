/* Reconstruction. Names/roles inferred, not original symbols. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: func_8014b870, func_8014b8d8 (and r2 func_8014be78, func_8014bee8): the original computes
   field_212 as sll 16 / sra 24 of the raw halfword; every form tried (>> 8, casts to s16/s8, int/unsigned/u16 locals,
   s16 pointer) folds to srl 8. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014b940(Object *object) {
    if (!(get_random_byte() & 1)) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014b98c(Object *object) {
    s16 *p = (s16 *)data_80189460;
    data_80189460 = (u16 *)(p + 1);
    if ((s16)object->meter >= *p) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014b9e8(Object *object) {
    if (object->other->active_state == 1) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014ba30(Object *object) {
    if (object->other->field_157 != 0) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014ba78(Object *object) {
    if (object->other->field_15b != 0) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014bac0(Object *object) {
    Object *other = object->other;
    if (other->link_flags == 0 || other->linked_object_3 == 0 || ((Object *)other->linked_object_3)->state_0 != 1) {
        func_8014c528(object);
    } else {
        func_8014c914(object);
    }
}
