/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void reset_round_fields_and_advance_step_2(GameState *state) {
    Attacker *a = (Attacker *)state;
    data_8018f5a0->step_index_2++;
    data_8018f5a0->pass_index = 0;
    data_801a6938 = 1;
    a->field_cc = 0;
    a->field_4e = 1;
    a->field_4f = 0;
    a->mode = 0;
    a->field_4b = 0;
    a->active_flag_2 = 0;
    a->field_66 = 0;
    a->field_6c = 0;
    a->field_64 = 0;
    a->field_75 = 0;
    a->a6_flag = 0;
    a->winner = 0;
    a->loser = 0;
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
    player_left.effect_code_bc = 0;
    player_left.effect_request = 0;
    player_left.sound_index = 0;
    player_left.field_bf = 0;
    player_right.effect_code_bc = 0;
    player_right.effect_request = 0;
    player_right.sound_index = 0;
    player_right.field_bf = 0;
    player_left.field_165 = 0;
    player_right.field_165 = 0;
    if (a->round_count == a->win_target * 2 - 1) {
        a->field_87 = 0xff;
    }
    if (a->special_off_flag == 0 && a->mode_marker == 0 && a->field_1b != 3 &&
        a->field_54 == a->compare_value && a->round_count == 0) {
        a->phase = 1;
        a->kind_code = player_left.kind;
        if ((a->field_1b & 1) == 0) {
            a->kind_code = player_right.kind;
        }
    }
    reset_counter_a_and_table_columns((GameState *)a);
}
