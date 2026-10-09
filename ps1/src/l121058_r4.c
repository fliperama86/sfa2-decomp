/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;
void func_80138410(GameState *state);

void func_80122230(GameState *state) {
    Attacker *a = (Attacker *)state;
    data_8018f5a0->field_50++;
    data_8018f5a0->field_52 = 0;
    data_801a6938 = 1;
    a->field_cc = 0;
    a->field_4e = 1;
    a->field_4f = 0;
    a->field_4c = 0;
    a->field_4b = 0;
    a->field_47 = 0;
    a->field_66 = 0;
    a->field_6c = 0;
    a->field_64 = 0;
    a->field_75 = 0;
    a->field_a6 = 0;
    a->field_78 = 0;
    a->field_7c = 0;
    a->field_138 = 0;
    a->field_139 = 0;
    a->field_13a = 0;
    a->field_13b = 0;
    a->field_2c = 0;
    a->field_6a = 0;
    a->field_11a = 0;
    a->field_ac = 0;
    a->field_ae = 0;
    a->field_88 = 0;
    a->field_63 = 0;
    a->field_12c = 0;
    a->field_12e = 0;
    a->field_dc |= 0xe;
    player_left.field_01 = 1;
    player_right.field_01 = 1;
    player_left.field_bc = 0;
    player_left.field_bd = 0;
    player_left.field_be = 0;
    player_left.field_bf = 0;
    player_right.field_bc = 0;
    player_right.field_bd = 0;
    player_right.field_be = 0;
    player_right.field_bf = 0;
    player_left.field_165 = 0;
    player_right.field_165 = 0;
    if (a->field_42 == a->field_48 * 2 - 1) {
        a->field_87 = 0xff;
    }
    if (a->field_83 == 0 && a->field_af == 0 && a->field_1b != 3 &&
        a->field_54 == a->field_a4 && a->field_42 == 0) {
        a->field_84 = 1;
        a->field_8b = player_left.kind;
        if ((a->field_1b & 1) == 0) {
            a->field_8b = player_right.kind;
        }
    }
    func_80138410((GameState *)a);
}
