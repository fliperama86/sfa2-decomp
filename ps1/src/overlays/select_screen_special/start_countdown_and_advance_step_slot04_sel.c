/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 data_801b9da4_slot04_sel;
extern u8 data_801b9c10_slot04_sel[];

void start_countdown_and_advance_step_slot04_sel(void) {
    HudState *h = data_8018f5a0;
    game_state.b0_flag = 1;
    game_state.field_80 = 0;
    game_state.selection_value = 0xff;
    game_state.b2_count = 0;
    data_801b9da4_slot04_sel = 0x40;
    h->step_index_2 = h->step_index_2 + 1;
}

void queue_item_and_count_down_to_next_step_slot04_sel(void) {
    u16 t;
    add_item_to_table(data_801b9c10_slot04_sel);
    t = data_801b9da4_slot04_sel - 1;
    data_801b9da4_slot04_sel = t;
    if (t == 0) {
        HudState *h = data_8018f5a0;
        h->step_index_2 = h->step_index_2 + 1;
    }
}

void advance_substep_reset_step_b_slot04_sel(void) {
    HudState *h = data_8018f5a0;
    game_state.field_09 = 0;
    h->step_index_2 = 0;
    h->substep_index = h->substep_index + 1;
}
