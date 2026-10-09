/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801213c0(GameState *g) {
    Object *l;
    Object *r;
    int z;
    z = 0;
    l = &player_left;
    data_8018f5a0->field_4c++;
    data_8018f5a0->field_4e = 0;
    g->field_c8 = 0;
    data_8018f5a0->field_50 = 0;
    g->field_ca = 0;
    data_8018f5a0->field_52 = 0;
    g->field_cc = 0;
    g->field_45 = 0;
    g->field_47 = 0;
    g->field_42 = 0;
    g->field_4d = 0;
    g->field_4b = 0;
    g->field_27 = 0;
    g->field_4f = 0;
    g->field_6a = 0;
    g->field_6c = 0;
    g->field_60 = 0;
    g->field_6d = 0;
    g->field_64 = 0;
    g->field_5c = 0;
    g->field_5d = 0;
    g->field_5e = 0;
    g->field_87 = 0;
    g->field_83 = 0;
    g->field_ab = 0;
    g->field_65 = 0;
    g->field_ae = 0;
    g->field_85 = 0;
    g->field_10c = 0;
    g->field_63 = 0;
    g->field_af = 0;
    g->field_84 = 0;
    g->field_09 = 1;
    g->field_8b = 0;
    g->field_4e = 0xff;
    g->field_2c = 0xff;
    g->field_a9 = 0;
    g->field_a4 = 7;
    g->field_28 = g->mode;
    g->field_5f = g->field_0e;
    g->field_48 = g->field_15;
    player_left.field_165 = 0;
    player_left.field_ce = 0;
    player_left.field_c6 = 0;
    player_left.field_bc = 0;
    player_left.field_bd = 0;
    player_left.field_be = 0;
    player_left.field_bf = 0;
    player_left.bytes_d0[0] = 0;
    player_left.bytes_d0[1] = 0;
    player_left.bytes_d0[2] = 0;
    player_left.bytes_d0[3] = 0;
    player_left.field_cc = 0;
    player_right.field_165 = 0;
    player_right.field_ce = 0;
    player_right.field_c6 = 0;
    player_right.field_bc = 0;
    player_right.field_bd = 0;
    player_right.field_be = 0;
    player_right.field_bf = 0;
    player_right.bytes_d0[0] = 0;
    player_right.bytes_d0[1] = 0;
    player_right.bytes_d0[2] = 0;
    player_right.bytes_d0[3] = 0;
    player_right.field_cc = 0;
    r = l + 1;
    if (g->mode == 3) {
        g->field_85 = 0xff;
        g->field_77 = 0;
        player_left.field_b8 = 0;
        player_right.field_b8 = 0;
    }
    g->field_4a = 0x3c;
    g->field_49 = g->field_13;
    if (g->field_2f == 0 && g->field_30 == 0) {
        func_801263d8(g);
        func_801262fc(g);
    }
    func_80123b88(g);
    func_80121680(g, l, r);
    g->field_2c = 1;
    func_8011eae4();
    func_8013839c(g);
    func_8014e890(game_state.field_40);
    func_8014eb9c(data_801a8067, data_801a83fb);
    func_80136dc4();
    func_80151020(0x604);
    func_801192bc(1);
    func_80157d00(1);
    data_80190568 = 1;
    data_801a6938 = z;
    func_80119340(2);
}
