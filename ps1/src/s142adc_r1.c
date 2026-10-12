/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80142adc(Object *object) {
    if (object->field_49 != 0 && object->active_flag_4 == 0 && func_80130470(object)) {
        func_8013047c(object);
    }
}

void func_80142b3c(Object *object) {
    func_80142c04(object);
    if (((object->pressed_bits | object->released_bits) & 0x80) || ((object->pressed_bits | object->released_bits) & 1)) {
        object->direction_index = 0;
    } else if ((object->pressed_bits | object->released_bits) & 0x10) {
        object->direction_index = 2;
    } else {
        object->direction_index = 4;
    }
}

void func_80142ba0(Object *object) {
    func_80142c04(object);
    if (((object->pressed_bits | object->released_bits) & 0x40) || ((object->pressed_bits | object->released_bits) & 2)) {
        object->direction_index = 0;
    } else if ((object->pressed_bits | object->released_bits) & 0x20) {
        object->direction_index = 2;
    } else {
        object->direction_index = 4;
    }
}

void func_80142c04(Object *object) {
    object->field_49 = 0;
    object->field_29c = 0;
    object->sequence_hold_flag = 0;
    if (object->field_7e != 0) object->field_49 = object->field_49 + 1;
    ref_other.p = object->other;
    if (object->sound_side != 0) ref_other.p->hit_count_b = 0;
    else ref_other.p->hit_count_a = 0;
}
