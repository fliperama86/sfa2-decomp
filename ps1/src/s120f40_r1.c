/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;

void func_80120f40(Object *object, u8 a1, u16 a2) {
    object->field_00 = 1;
    object->field_cd = 1;
    object->field_02 = a1;
    object->side = a1;
    object->field_d4 = 0;
    object->field_d7 = 0;
    object->field_c0 = 0;
    object->field_124 = 0;
    object->field_db = 0;
    object->field_ef = 0;
    object->field_ec = 0;
    object->field_ed = 0;
    object->field_ee = 0;
    object->field_c1 = 0;
    object->field_11a = 0;
    object->field_f1 = 0;
    object->field_fc = 0x90;
    object->field_fe = 0;
    object->field_c2 = a2;
}

void func_80120f98(GameState *state) {
    if (data_801ac61c == 0 || data_801a89f0 == 0) {
        table_8016e738[data_8018f5a0->field_4a](state);
    }
}

void func_80121004(GameState *state) {
    data_8018f5a0->field_4a++;
    state->field_1a = 1;
    state->field_64 = 0;
    player_left.field_01 = 0;
    player_right.field_01 = 0;
    func_8011eae4();
}
