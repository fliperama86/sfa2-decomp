/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u16 func_80130470(Object *object) {
    return (u8)object->pressed_bits;
}


void func_8013047c(Object *object) {
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 5;
    object->state_3 = 0;
    if (object->field_7e == 0) object->facing_side = object->saved_facing;
    func_80142c04(object);
    object->active_flag = 0;
    func_80130504(object);
    object->field_157 = ((u8)func_8012f618(object) != 0) << 1;
    object->input_dir_0 = object->field_157;
    func_8012b45c(object);
}

void func_80130504(Object *object) {
    BytePair d = func_8013054c(object);
    object->direction_index = d.first;
    object->input_dir_second = d.second;
}

BytePair func_8013054c(Object *object) {
    BytePair d;
    u16 pad = object->pressed_bits;
    d.second = 0;
    d.first = 0;
    if (pad & 0x80) {
        d.first = 0;
        d.second = 0;
    } else if (pad & 0x10) {
        d.first = 2;
        d.second = 0;
    } else if (pad & 0x4) {
        d.first = 4;
        d.second = 0;
    } else if (pad & 0x40) {
        d.first = 0;
        d.second = 2;
    } else if (pad & 0x20) {
        d.first = 2;
        d.second = 2;
    } else if (pad & 0x8) {
        d.first = 4;
        d.second = 2;
    } else if (pad & 0x1) {
        d.first = 0;
        d.second = 0;
    } else if (pad & 0x2) {
        d.first = 0;
        d.second = 2;
    }
    return d;
}
