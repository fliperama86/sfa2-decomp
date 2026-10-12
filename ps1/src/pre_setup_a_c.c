/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80130280(Object *object) {
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 3;
    object->state_3 = 0;
    object->field_157 = 0;
    func_80130088(object);
    object->variant_index = 1;
    if (!(object->input_bits_2 & 0x8000)) {
        object->variant_index = 0xff;
        if (!(object->input_bits_2 & 0x2000)) object->variant_index = 0;
    }
    set_sequence_by_side(object, 0x10);
}

void func_80130304(Object *object) {
    func_80130ec0(object);
    if (object->side == 0) {
        if (object->mode_flag_2 == 0) scratch_fn_a_left(object);
        else scratch_fn_b_left(object);
    } else {
        if (object->mode_flag_2 == 0) scratch_fn_a_right(object);
        else scratch_fn_b_right(object);
    }
}

void func_801303a0(Object *object) {
    object->effect_request = 4;
    object->field_256 = 1;
    object->table_flag = 1;
    func_80141f28(object, -0x30);
    game_state.other_object = object->other;
    if (game_state.other_object->active_state != 0) {
        if (object->side == 0) scratch_fn_c_left(object);
        else scratch_fn_c_right(object);
    } else {
        if (object->side == 0) scratch_fn_d_left(object);
        else scratch_fn_d_right(object);
    }
}
