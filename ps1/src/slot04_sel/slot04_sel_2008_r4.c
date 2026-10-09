/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801b9d64_slot04_sel;

void func_801b2480_slot04_sel(void) {
    HudState *h = data_8018f5a0;
    game_state.field_b0 = 1;
    game_state.field_80 = 0;
    game_state.field_b1 = 0xff;
    game_state.field_b2 = 0;
    data_801b9d64_slot04_sel = 1;
    h->field_50++;
}

void func_801b24cc_slot04_sel(void) {
    int t = data_801b9d64_slot04_sel - 1;
    data_801b9d64_slot04_sel = t;
    if ((u16)t == 0) {
        data_8018f5a0->field_50++;
    }
}

void func_801b2514_slot04_sel(void) {
    HudState *h = data_8018f5a0;
    game_state.field_09 = 0;
    h->field_50 = 0;
    h->field_4e++;
}
