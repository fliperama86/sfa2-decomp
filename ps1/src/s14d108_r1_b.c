/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_8014de94(Object *object);
u8 func_8014e758(Object *object);
u8 func_8014e718(Object *object);

void func_8014d5e0(Object *object) {
    if (func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->field_06 == 0 || object->field_21c == 0
        || (object->field_21c--, object->field_48 != 0 && object->field_164 != 0)) {
        func_8014d99c(object);
    }
}

void func_8014d66c(Object *object) {
    if (func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->field_06 == 0 || object->field_21c == 0
        || (object->field_21c--, (u16)(object->field_211 | (object->field_210 << 8)) >= object->field_21e)
        || (object->field_48 != 0 && object->field_164 != 0)) {
        func_8014d99c(object);
    }
}

void func_8014d720(Object *object) {
    if (func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->field_06 == 0 || object->field_21c == 0
        || (object->field_21c--, (u16)(object->field_211 | (object->field_210 << 8)) < object->field_21e)
        || (object->field_48 != 0 && object->field_164 != 0)) {
        func_8014d99c(object);
    }
}
