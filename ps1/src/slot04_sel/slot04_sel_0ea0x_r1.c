/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801b6e70_slot04_sel[])(Object *, Slot04SelRec *);
extern TextItem *data_801b6fcc_slot04_sel[];
extern TextItem *data_801b701c_slot04_sel[];
extern u8 data_801b7648_slot04_sel[];
extern TextItem data_801b7978_slot04_sel;
extern s16 data_801b797c_slot04_sel;
extern s16 data_801b797e_slot04_sel;
extern Slot04SelRec data_801b9cf0_slot04_sel[];
extern u8 data_801b9d28_slot04_sel;
extern u8 data_801b9d2c_slot04_sel;

/* Inferred: the n-th player's object (player_left and player_right lie one after the other). */
static inline Object *player_at(int n) {
    return &player_left + n;
}

/*
 * What it does (inferred): the per-frame update of a two-player selection
 * screen. Draws the text blocks chosen by each player's side, then runs for
 * each player the handler picked by its selection record's state, and
 * clears the records when both agree on a choice or the screen state changed.
 */
void func_801b0ea0_slot04_sel(void) {
    Slot04SelRec *rec0;
    Slot04SelRec *rec1;

    func_801519b4(data_801b701c_slot04_sel[player_at(0)->side]);
    func_801519b4(data_801b701c_slot04_sel[player_at(1)->side]);
    func_801519b4(data_801b6fcc_slot04_sel[player_at(0)->side]);
    func_801519b4(data_801b6fcc_slot04_sel[player_at(1)->side]);
    data_801b797c_slot04_sel = 0xa8;
    data_801b797e_slot04_sel = 0x30;
    func_801519b4(&data_801b7978_slot04_sel);
    func_801519b4(data_801b7648_slot04_sel);
    rec0 = data_801b9cf0_slot04_sel;
    data_801b6e70_slot04_sel[rec0->state](player_at(0), rec0);
    rec1 = rec0 + 1;
    data_801b6e70_slot04_sel[rec1->state](player_at(1), rec1);
    if (game_state.field_07 == (rec0->field_09 | rec1->field_09) && game_state.field_07 != 0 && (rec0->field_0b != 0 || rec1->field_0b != 0)) {
        rec0->state = 0;
        rec1->state = 0;
        data_8018f5a0->field_50 = 0;
        data_8018f5a0->field_4e++;
    }
    if (data_801b9d28_slot04_sel != game_state.field_07 || data_801b9d2c_slot04_sel != game_state.mode) {
        rec0->state = 0;
        rec1->state = 0;
        data_8018f5a0->field_50 = 1;
    }
}
