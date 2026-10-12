/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80122e54(GameState *g) {
    data_8018f5a0->substep_index = 0;
    g->timer_or_bits = 0;
    data_8018f5a0->step_index_2 = 0;
    g->countdown_timer_2 = 0;
    data_8018f5a0->pass_index = 0;
    g->field_cc = 0;
    g->field_4d = 0;
    g->field_4e = 0;
    g->side_bits_2 = 0;
    g->active_flag = 0;
    g->field_09 = 1;
    g->gate_flag = 1;
    g->field_4f = 1;
    g->field_65 = 0xff;
    player_left.win_count = 0;
    player_right.win_count = 0;
    player_left.meter = 0;
    player_right.meter = 0;
    if (player_left.kind == 0x13) {
        player_left.kind = 0x11;
    }
    if (player_right.kind == 0x13) {
        player_right.kind = 0x11;
    }
    data_80190568 = 0;
    g->init_timer_value = 0x3c;
    g->round_time = g->table_value_b;
    func_8011eae4();
    func_80136c8c();
    func_8013245c();
    func_801285e0();
    func_80120408();
    if (*(u16 *)data_801a6984 != 0) {
        data_80197f1c = 0xff;
    }
}
