/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801b9d24_slot04_sel;

void func_801b028c_slot04_sel(void) {
    game_state.b0_flag = 1;
    game_state.field_80 = 0;
    game_state.selection_value = 0xff;
    game_state.b2_count = 0;
    data_801b9d24_slot04_sel = 1;
    data_8018f5a0->step_index_2 += 1;
}

void func_801b02d8_slot04_sel(void) {
    if (--data_801b9d24_slot04_sel == 0) {
        data_8018f5a0->step_index_2 += 1;
    }
}

void func_801b0320_slot04_sel(void) {
    game_state.field_09 = 0;
    data_8018f5a0->step_index_2 = 0;
    data_8018f5a0->substep_index += 1;
}
