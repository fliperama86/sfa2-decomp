/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014d108(Object *object) {
    if (object->countdown_3 == 0 || --object->countdown_3 == 0) {
        func_8014d99c(object);
    }
}

void func_8014d14c(Object *object) {
    if ((u8)func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->link_flags == 0 || object->linked_object_3 == 0) {
        func_8014d99c(object);
    }
}

void func_8014d1b8(Object *object) {
    if ((u8)func_8014de94(object)) {
        func_8014e080(object);
    } else if ((u8)func_8014e758(object)) {
        func_8014d99c(object);
    }
}

void func_8014d218(Object *object) {
    int hi, lo;
    if ((u8)func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->countdown_3 == 0 || --object->countdown_3 == 0
        || (hi = object->limit_hi, lo = object->limit_lo, !((s16)((hi << 8) + lo) < (s16)object->compare_limit))) {
        func_8014d99c(object);
    }
}

void func_8014d2b0(Object *object) {
    int hi, lo;
    if ((u8)func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->countdown_3 == 0 || --object->countdown_3 == 0
        || (hi = object->limit_hi, lo = object->limit_lo, (s16)((hi << 8) + lo) < (s16)object->compare_limit)) {
        func_8014d99c(object);
    }
}

void func_8014d348(Object *object) {
    if ((u8)func_8014de94(object)) {
        func_8014e080(object);
    } else if ((u8)func_8014e718(object)) {
        func_8014d99c(object);
    }
}

void func_8014d3a8(Object *object) {
    data_8017d2e0[object->field_209](object);
}

void func_8014d3e8(Object *object) {
    if ((u8)func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->state_2 == 0
        || (u16)(object->limit_lo | (object->limit_hi << 8)) >= object->compare_limit
        || (object->variant_index != 0 && object->bound_push_flag != 0)) {
        func_8014d99c(object);
    }
}

void func_8014d484(Object *object) {
    if ((u8)func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->state_2 == 0
        || (u16)(object->limit_lo | (object->limit_hi << 8)) < object->compare_limit
        || (object->variant_index != 0 && object->bound_push_flag != 0)) {
        func_8014d99c(object);
    }
}
