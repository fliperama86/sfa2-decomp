/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80132070(Object *object) {
    if ((*(u32 *)&object->state_0 & 0xffffff) == 0x70001 &&
        object->field_15a == 1 && object->field_7e == 0 &&
        object->active_flag != 0) {
        object->active_flag = 0;
        func_801307e0(object, 0x28);
    }
}

int func_801320e8(Object *object) {
    int result;
    if (object->mode_flag_2 != 0) {
        result = 0;
    } else if (!(u8)func_8013d0fc(object)) {
        result = 0;
    } else {
        result = (u8)func_801418bc(object) != 0;
    }
    return result;
}

void func_80132140(Object *object) {
    object->variant_index = 1;
    if (!(object->input_bits_2 & 0x8000)) object->variant_index = 0xff;
    object->field_15a = 2;
    object->active_flag_3 = 0;
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 7;
    object->state_3 = 0;
    func_80132198(object);
}

void func_80132198(Object *object) {
    u8 side = object->saved_facing;
    u16 bits = object->pressed_bits | object->released_bits;
    u8 value;
    object->substate_index = 0;
    object->field_12d = 0;
    object->step_count_2 = 0;
    object->step_limit = 0;
    object->facing_side = side;
    if (bits & 0xc0) {
        value = 0;
    } else if (bits & 0x30) {
        value = 2;
    } else {
        value = 4;
    }
    object->direction_index = value;
}
