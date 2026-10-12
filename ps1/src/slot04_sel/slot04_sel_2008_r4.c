/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801b9d64_slot04_sel;

void func_801b2480_slot04_sel(void) {
    HudState *h = data_8018f5a0;
    game_state.b0_flag = 1;
    game_state.field_80 = 0;
    game_state.selection_value = 0xff;
    game_state.b2_count = 0;
    data_801b9d64_slot04_sel = 1;
    h->step_index_2++;
}

void func_801b24cc_slot04_sel(void) {
    int t = data_801b9d64_slot04_sel - 1;
    data_801b9d64_slot04_sel = t;
    if ((u16)t == 0) {
        data_8018f5a0->step_index_2++;
    }
}

void func_801b2514_slot04_sel(void) {
    HudState *h = data_8018f5a0;
    game_state.field_09 = 0;
    h->step_index_2 = 0;
    h->substep_index++;
}
