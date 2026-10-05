/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014c178(Object *object) {
    u16 v = *data_80189460++;
    Object *other = object->other;
    if ((u16)(other->field_70 - v) >= (u16)other->pos_y) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014c1e8(Object *object) {
    Object *ref;
    u8 v;
    if (object->other->field_240 == 0 || (ref = (Object *)object->other->field_14c) == 0
        || (v = table_8017d0ac[(ref->field_02 << 4) + (ref->field_ac >> 1)]) == 0 || v == 4) {
        func_8014c914();
    } else {
        func_8014c528();
    }
}

void func_8014c278(Object *object) {
    Object *ref;
    if (object->other->field_240 == 0 || (ref = (Object *)object->other->field_14c) == 0
        || table_8017d0ac[(ref->field_02 << 4) + (ref->field_ac >> 1)] < 3) {
        func_8014c528();
    } else {
        func_8014c914();
    }
}

void func_8014c304(Object *object) {
    s16 v = *(s16 *)data_80189460++;
    if ((s16)object->field_5c < v) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}

void func_8014c360(Object *object) {
    if ((func_80151184(object) & 3) == 0) {
        func_8014c914(object);
    } else {
        func_8014c528(object);
    }
}
