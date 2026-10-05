/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80126a00(GameState *state, Object *object) {
    object->field_a9 = 3;
    object->field_aa = 0;
    object->field_ab = 0;
    object->field_ac = 0;
    *(u16 *)&object->field_ae = 0;
    object->field_b0 = 0;
    object->field_b2 = 0;
    object->field_a5 = 2;
    object->field_cd = 0;
    object->field_ce = 0;
    object->field_d7 = 0;
    object->field_c0 = 0;
    object->field_124 = 0;
    object->field_bc = 0;
    object->field_bd = 0;
    object->field_be = 0;
    object->field_bf = 0;
    object->field_e0 = 0;
    object->kind = object->field_118;
    func_8011ef88(object);
    if (object->side == 0) {
        state->field_17 |= 1;
        state->field_07 |= 1;
    } else if (object->side == 1) {
        state->field_17 |= 2;
        state->field_07 |= 2;
    }
    state->field_1c = 1;
}
