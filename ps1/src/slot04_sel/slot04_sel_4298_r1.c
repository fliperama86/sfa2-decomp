/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern TextItem *data_801b8978_slot04_sel[];
extern Slot04SelRec8978 data_801b89d4_slot04_sel[];
extern s16 data_801b89d8_slot04_sel;
extern s16 data_801b89da_slot04_sel;
extern Slot04SelRec8c00 *data_801b8c00_slot04_sel[];
extern u16 data_801b9d64_slot04_sel;
extern u8 data_8016e68f[];
extern u8 data_8016e690[];
void func_801b4424_slot04_sel(void);

void func_801b4298_slot04_sel(void) {
    game_state.field_09 = 0xff;
    data_801b9d64_slot04_sel = 0x40;
    data_8018f5a0->field_50++;
}

void func_801b42c8_slot04_sel(void) {
    TextItem *r;
    Slot04SelRec8c00 *s;
    u16 t;

    r = data_801b8978_slot04_sel[data_801a8067];
    r->field_04 = 0x30;
    r->field_06 = 0x50;
    func_801519b4(r);
    data_801b89d8_slot04_sel = 0xa0;
    data_801b89da_slot04_sel = 0x68;
    func_801519b4(data_801b89d4_slot04_sel);
    s = data_801b8c00_slot04_sel[data_801a83fb];
    s->field_04 = 0xc0;
    s->field_06 = 0x80;
    func_801519b4(s);
    t = data_801b9d64_slot04_sel - 1;
    data_801b9d64_slot04_sel = t;
    if (t == 0) {
        data_8018f5a0->field_50++;
    }
}

void func_801b43ac_slot04_sel(void) {
    game_state.field_09 = 0;
    game_state.field_1a = 0;
    game_state.field_07 = 0;
    func_801b4424_slot04_sel();
    data_8018f5a0->field_4c = 2;
    data_8018f5a0->field_4e = 0;
    data_8018f5a0->field_50 = 0;
    data_8018f5a0->field_52 = 0;
    data_8018f5a0->field_54 = 0;
    player_left.field_cd = data_8016e68f[0];
    player_right.field_cd = data_8016e690[0];
}
