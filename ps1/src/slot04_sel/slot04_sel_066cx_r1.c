/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_801b6d88_slot04_sel[])(Object *, Slot04SelRec *);
extern u8 data_801b7034_slot04_sel[];
extern u8 data_801b7058_slot04_sel[];
extern u8 data_801b707c_slot04_sel[];
extern u8 data_801b70a0_slot04_sel[];
extern u8 data_801b70c4_slot04_sel[];
extern u8 data_801b70e8_slot04_sel[];
extern u8 data_801b710c_slot04_sel[];
extern u8 data_801b7130_slot04_sel[];
extern u8 data_801b7154_slot04_sel[];
extern u8 data_801b7178_slot04_sel[];
extern u8 data_801b719c_slot04_sel[];
extern u8 data_801b71c0_slot04_sel[];
extern u8 data_801b71e4_slot04_sel[];
extern u8 data_801b7208_slot04_sel[];
extern u8 data_801b722c_slot04_sel[];
extern u8 data_801b7250_slot04_sel[];
extern u8 data_801b7274_slot04_sel[];
extern u8 data_801b7298_slot04_sel[];
extern u8 data_801b7364_slot04_sel[];
extern TextItem data_801b7978_slot04_sel;
extern s16 data_801b797c_slot04_sel;
extern s16 data_801b797e_slot04_sel;
extern Slot04SelRec data_801b9cf0_slot04_sel[];
extern u8 data_801b9d28_slot04_sel;
extern u8 data_801b9d2c_slot04_sel;

/* The local w holds the first player and then the second record. Written with one local for each, this function differs from the original in 2 instruction slots. */
void func_801b066c_slot04_sel(void) {
    Slot04SelRec *r;
    Object *second;
    void *w;

    func_801519b4(data_801b7364_slot04_sel);
    data_801b797c_slot04_sel = 0xa8;
    data_801b797e_slot04_sel = 0x30;
    func_801519b4(&data_801b7978_slot04_sel);
    func_801519b4(data_801b7034_slot04_sel);
    func_801519b4(data_801b7058_slot04_sel);
    func_801519b4(data_801b707c_slot04_sel);
    func_801519b4(data_801b70a0_slot04_sel);
    func_801519b4(data_801b70c4_slot04_sel);
    func_801519b4(data_801b70e8_slot04_sel);
    func_801519b4(data_801b710c_slot04_sel);
    func_801519b4(data_801b7130_slot04_sel);
    func_801519b4(data_801b7154_slot04_sel);
    func_801519b4(data_801b7178_slot04_sel);
    func_801519b4(data_801b719c_slot04_sel);
    func_801519b4(data_801b71c0_slot04_sel);
    func_801519b4(data_801b71e4_slot04_sel);
    func_801519b4(data_801b7208_slot04_sel);
    func_801519b4(data_801b722c_slot04_sel);
    func_801519b4(data_801b7250_slot04_sel);
    func_801519b4(data_801b7274_slot04_sel);
    func_801519b4(data_801b7298_slot04_sel);
    w = &player_left;
    r = data_801b9cf0_slot04_sel;
    data_801b6d88_slot04_sel[r->field_00](w, r);
    second = (Object *)w + 1;
    w = r + 1;
    data_801b6d88_slot04_sel[r[1].field_00](second, w);
    if (game_state.field_07 == (data_801b9cf0_slot04_sel[0].field_04 | data_801b9cf0_slot04_sel[1].field_04)) {
        r[0].field_00 = 1;
        r[1].field_00 = 1;
        data_8018f5a0->field_50++;
    }
    data_801b9d28_slot04_sel = game_state.field_07;
    data_801b9d2c_slot04_sel = game_state.mode;
}
