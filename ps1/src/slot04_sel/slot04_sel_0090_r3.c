/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u16 data_801b9d24_slot04_sel;

void func_801b028c_slot04_sel(void) {
    game_state.field_b0 = 1;
    game_state.field_80 = 0;
    game_state.field_b1 = 0xff;
    game_state.field_b2 = 0;
    data_801b9d24_slot04_sel = 1;
    data_8018f5a0->field_50 += 1;
}

void func_801b02d8_slot04_sel(void) {
    if (--data_801b9d24_slot04_sel == 0) {
        data_8018f5a0->field_50 += 1;
    }
}

void func_801b0320_slot04_sel(void) {
    game_state.field_09 = 0;
    data_8018f5a0->field_50 = 0;
    data_8018f5a0->field_4e += 1;
}
