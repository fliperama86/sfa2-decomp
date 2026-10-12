/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801b9da4_slot04_sel;
extern u8 data_801b9c10_slot04_sel[];

void func_801b4cb4_slot04_sel(void) {
    HudState *h = data_8018f5a0;
    game_state.b0_flag = 1;
    game_state.field_80 = 0;
    game_state.selection_value = 0xff;
    game_state.b2_count = 0;
    data_801b9da4_slot04_sel = 0x40;
    h->step_index_2 = h->step_index_2 + 1;
}

void func_801b4d00_slot04_sel(void) {
    u16 t;
    func_801519b4(data_801b9c10_slot04_sel);
    t = data_801b9da4_slot04_sel - 1;
    data_801b9da4_slot04_sel = t;
    if (t == 0) {
        HudState *h = data_8018f5a0;
        h->step_index_2 = h->step_index_2 + 1;
    }
}

void func_801b4d68_slot04_sel(void) {
    HudState *h = data_8018f5a0;
    game_state.field_09 = 0;
    h->step_index_2 = 0;
    h->substep_index = h->substep_index + 1;
}
