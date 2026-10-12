/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014d5e0(Object *object) {
    if ((u8)func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->state_2 == 0 || object->countdown_3 == 0
        || (object->countdown_3--, object->variant_index != 0 && object->bound_push_flag != 0)) {
        func_8014d99c(object);
    }
}

void func_8014d66c(Object *object) {
    if ((u8)func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->state_2 == 0 || object->countdown_3 == 0
        || (object->countdown_3--, (u16)(object->limit_lo | (object->limit_hi << 8)) >= object->compare_limit)
        || (object->variant_index != 0 && object->bound_push_flag != 0)) {
        func_8014d99c(object);
    }
}

void func_8014d720(Object *object) {
    if ((u8)func_8014de94(object)) {
        func_8014e080(object);
    } else if (object->state_2 == 0 || object->countdown_3 == 0
        || (object->countdown_3--, (u16)(object->limit_lo | (object->limit_hi << 8)) < object->compare_limit)
        || (object->variant_index != 0 && object->bound_push_flag != 0)) {
        func_8014d99c(object);
    }
}
