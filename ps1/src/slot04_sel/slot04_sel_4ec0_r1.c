/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_801b8cc8_slot04_sel[])(Object *, Object *, Slot04SelRec9d78 *);
extern Object *data_801b8f74_slot04_sel[];
extern Object *data_801b8fc4_slot04_sel[];
extern u8 data_801b8fe0_slot04_sel[];
extern u8 data_801b9004_slot04_sel[];
extern u8 data_801b9028_slot04_sel[];
extern u8 data_801b904c_slot04_sel[];
extern u8 data_801b9070_slot04_sel[];
extern u8 data_801b9094_slot04_sel[];
extern u8 data_801b90b8_slot04_sel[];
extern u8 data_801b90dc_slot04_sel[];
extern u8 data_801b9100_slot04_sel[];
extern u8 data_801b9124_slot04_sel[];
extern u8 data_801b9148_slot04_sel[];
extern u8 data_801b916c_slot04_sel[];
extern u8 data_801b9190_slot04_sel[];
extern u8 data_801b91b4_slot04_sel[];
extern u8 data_801b91d8_slot04_sel[];
extern u8 data_801b91fc_slot04_sel[];
extern u8 data_801b9220_slot04_sel[];
extern u8 data_801b9244_slot04_sel[];
extern u8 data_801b9310_slot04_sel[];
extern u8 data_801b9330_slot04_sel[];
extern u8 data_801b9358_slot04_sel[];
extern Slot04SelRec9d78 data_801b9d78_slot04_sel[];
extern u8 data_801b9da8_slot04_sel;
extern u8 data_801b9dac_slot04_sel;

void func_801b4ec0_slot04_sel(void) {
    Object *l = &player_left;
    Object *r = l + 1;
    Slot04SelRec9d78 *a = data_801b9d78_slot04_sel;
    Slot04SelRec9d78 *b = a + 1;
    func_801519b4((Object *)data_801b9310_slot04_sel);
    if (data_8018f5a0->field_50 == 1) {
        func_801519b4((Object *)data_801b9330_slot04_sel);
    } else if (data_8018f5a0->field_50 == 2) {
        func_801519b4((Object *)data_801b9358_slot04_sel);
    }
    func_801519b4((Object *)(data_801b8fe0_slot04_sel + 0));
    func_801519b4((Object *)data_801b9004_slot04_sel);
    func_801519b4((Object *)data_801b9028_slot04_sel);
    func_801519b4((Object *)data_801b904c_slot04_sel);
    func_801519b4((Object *)data_801b9070_slot04_sel);
    func_801519b4((Object *)data_801b9094_slot04_sel);
    func_801519b4((Object *)data_801b90b8_slot04_sel);
    func_801519b4((Object *)data_801b90dc_slot04_sel);
    func_801519b4((Object *)data_801b9100_slot04_sel);
    func_801519b4((Object *)data_801b9124_slot04_sel);
    func_801519b4((Object *)data_801b9148_slot04_sel);
    func_801519b4((Object *)data_801b916c_slot04_sel);
    func_801519b4((Object *)data_801b9190_slot04_sel);
    func_801519b4((Object *)data_801b91b4_slot04_sel);
    func_801519b4((Object *)data_801b91d8_slot04_sel);
    func_801519b4((Object *)data_801b91fc_slot04_sel);
    func_801519b4((Object *)data_801b9220_slot04_sel);
    func_801519b4((Object *)data_801b9244_slot04_sel);
    data_801b8cc8_slot04_sel[a->field_00](l, r, a);
    data_801b8cc8_slot04_sel[b->field_00](r, l, b);
    if (data_8018f5a0->field_50 == 1) {
        if (game_state.field_07 == (a->field_04 | b->field_04)) {
            a->field_00 = 0;
            b->field_00 = 0;
            a->field_04 = 0;
            b->field_04 = 0;
            a->field_14 = 0;
            b->field_14 = 0;
            a->field_15 = 0;
            b->field_15 = 0;
            data_8018f5a0->field_50 = data_8018f5a0->field_50 + 1;
        }
    } else if (data_8018f5a0->field_50 == 2) {
        Object *o1 = data_801b8fc4_slot04_sel[game_state.field_07];
        Object *o0 = data_801b8f74_slot04_sel[game_state.field_07];
        func_801519b4(o1);
        func_801519b4(o0);
        if (game_state.field_07 == (a->field_04 | b->field_04)) {
            a->field_00 = 0;
            b->field_00 = 0;
            l->field_cd = 0;
            r->field_cd = 0;
            data_8018f5a0->field_4e = data_8018f5a0->field_4e + 1;
            data_8018f5a0->field_50 = 0;
            game_state.field_40 = (u8)func_80125afc(game_state.field_b1);
        }
    }
    data_801b9da8_slot04_sel = game_state.field_07;
    data_801b9dac_slot04_sel = game_state.mode;
}
