/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern TextItem *data_801b9908_slot04_sel[];
extern TextItem *data_801b9b90_slot04_sel[];
extern TextItem data_801b9964_slot04_sel[];
extern u16 data_801b9da4_slot04_sel;

void func_801b5f68_slot04_sel(void) {
    game_state.field_09 = 0xff;
    data_801b9da4_slot04_sel = 0x40;
    data_8018f5a0->field_50++;
}

void func_801b5f98_slot04_sel(void) {
    TextItem *p;
    int t;

    p = data_801b9908_slot04_sel[player_left.kind];
    p->field_04 = 0x30;
    p->field_06 = 0x50;
    func_801519b4((Object *)p);
    p = data_801b9964_slot04_sel;
    data_801b9964_slot04_sel[0].field_04 = 0xa0;
    data_801b9964_slot04_sel[0].field_06 = 0x68;
    func_801519b4((Object *)p);
    p = data_801b9b90_slot04_sel[player_right.kind];
    p->field_04 = 0xc0;
    p->field_06 = 0x80;
    func_801519b4((Object *)p);
    t = data_801b9da4_slot04_sel - 1;
    data_801b9da4_slot04_sel = t;
    if ((u16)t == 0) {
        data_8018f5a0->field_50++;
    }
}

void func_801b607c_slot04_sel(void) {
    game_state.field_09 = 0;
    game_state.field_1a = 0;
    game_state.field_07 = 0;
    data_8018f5a0->field_4c = 2;
    data_8018f5a0->field_4e = 0;
    data_8018f5a0->field_50 = 0;
    data_8018f5a0->field_52 = 0;
    data_8018f5a0->field_54 = 0;
}
