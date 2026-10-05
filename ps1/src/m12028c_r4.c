/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;
extern u16 data_801a6984;


void func_80122e54(GameState *g) {
    data_8018f5a0->field_4e = 0;
    g->field_c8 = 0;
    data_8018f5a0->field_50 = 0;
    g->field_ca = 0;
    data_8018f5a0->field_52 = 0;
    g->field_cc = 0;
    g->field_4d = 0;
    g->field_4e = 0;
    g->field_4b = 0;
    g->field_27 = 0;
    g->field_09 = 1;
    g->field_6a = 1;
    g->field_4f = 1;
    g->field_65 = 0xff;
    player_left.field_ce = 0;
    player_right.field_ce = 0;
    player_left.field_c6 = 0;
    player_right.field_c6 = 0;
    if (player_left.kind == 0x13) {
        player_left.kind = 0x11;
    }
    if (player_right.kind == 0x13) {
        player_right.kind = 0x11;
    }
    data_80190568 = 0;
    g->field_4a = 0x3c;
    g->field_49 = g->field_13;
    func_8011eae4();
    func_80136c8c();
    func_8013245c();
    func_801285e0();
    func_80120408();
    if (data_801a6984 != 0) {
        data_80197f1c = 0xff;
    }
}
