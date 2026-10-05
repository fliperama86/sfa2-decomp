/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_8014de94(Object *object);
u8 func_8014e758(Object *object);
u8 func_8014e718(Object *object);

void func_8014d108(Object *object) {
    if (object->field_21c == 0 || --object->field_21c == 0) {
        func_8014d99c(object);
    }
}

void func_8014d14c(Object *object) {
    if (func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->field_240 == 0 || object->field_14c == 0) {
        func_8014d99c(object);
    }
}

void func_8014d1b8(Object *object) {
    if (func_8014de94(object)) {
        func_8014e080(object);
    } else if (func_8014e758(object)) {
        func_8014d99c(object);
    }
}

void func_8014d218(Object *object) {
    int hi, lo;
    if (func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->field_21c == 0 || --object->field_21c == 0
        || (hi = object->field_210, lo = object->field_211, !((s16)((hi << 8) + lo) < (s16)object->field_21e))) {
        func_8014d99c(object);
    }
}

void func_8014d2b0(Object *object) {
    int hi, lo;
    if (func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->field_21c == 0 || --object->field_21c == 0
        || (hi = object->field_210, lo = object->field_211, (s16)((hi << 8) + lo) < (s16)object->field_21e)) {
        func_8014d99c(object);
    }
}

void func_8014d348(Object *object) {
    if (func_8014de94(object)) {
        func_8014e080(object);
    } else if (func_8014e718(object)) {
        func_8014d99c(object);
    }
}

void func_8014d3a8(Object *object) {
    data_8017d2e0[object->field_209](object);
}

void func_8014d3e8(Object *object) {
    if (func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->field_06 == 0
        || (u16)(object->field_211 | (object->field_210 << 8)) >= object->field_21e
        || (object->field_48 != 0 && object->field_164 != 0)) {
        func_8014d99c(object);
    }
}

void func_8014d484(Object *object) {
    if (func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->field_06 == 0
        || (u16)(object->field_211 | (object->field_210 << 8)) < object->field_21e
        || (object->field_48 != 0 && object->field_164 != 0)) {
        func_8014d99c(object);
    }
}
