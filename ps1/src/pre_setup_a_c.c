/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80130280(Object *object) {
    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 3;
    object->field_07 = 0;
    object->field_157 = 0;
    func_80130088(object);
    object->field_48 = 1;
    if (!(object->field_130 & 0x8000)) {
        object->field_48 = 0xff;
        if (!(object->field_130 & 0x2000)) object->field_48 = 0;
    }
    func_80130678(object, 0x10);
}

void func_80130304(Object *object) {
    func_80130ec0(object);
    if (object->side == 0) {
        if (object->field_cd == 0) scratch_fn_a_left(object);
        else scratch_fn_b_left(object);
    } else {
        if (object->field_cd == 0) scratch_fn_a_right(object);
        else scratch_fn_b_right(object);
    }
}

void func_801303a0(Object *object) {
    object->field_bd = 4;
    object->field_256 = 1;
    object->field_257 = 1;
    func_80141f28(object, -0x30);
    game_state.field_358 = object->other;
    if (game_state.field_358->field_45 != 0) {
        if (object->side == 0) scratch_fn_c_left(object);
        else scratch_fn_c_right(object);
    } else {
        if (object->side == 0) scratch_fn_d_left(object);
        else scratch_fn_d_right(object);
    }
}
