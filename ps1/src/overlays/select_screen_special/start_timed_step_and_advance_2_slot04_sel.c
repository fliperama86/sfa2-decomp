/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern TextItem *data_801b9908_slot04_sel[];
extern TextItem *data_801b9b90_slot04_sel[];
extern TextItem data_801b9964_slot04_sel[];
extern u16 data_801b9da4_slot04_sel;

void start_timed_step_and_advance_2_slot04_sel(void) {
    game_state.field_09 = 0xff;
    data_801b9da4_slot04_sel = 0x40;
    data_8018f5a0->step_index_2++;
}

void draw_versus_text_items_with_timer_slot04_sel(void) {
    TextItem *p;
    int t;

    p = data_801b9908_slot04_sel[player_left.kind];
    p->x = 0x30;
    p->y = 0x50;
    add_item_to_table(p);
    p = data_801b9964_slot04_sel;
    data_801b9964_slot04_sel[0].x = 0xa0;
    data_801b9964_slot04_sel[0].y = 0x68;
    add_item_to_table(p);
    p = data_801b9b90_slot04_sel[player_right.kind];
    p->x = 0xc0;
    p->y = 0x80;
    add_item_to_table(p);
    t = data_801b9da4_slot04_sel - 1;
    data_801b9da4_slot04_sel = t;
    if ((u16)t == 0) {
        data_8018f5a0->step_index_2++;
    }
}

void reset_step_indices_to_step_2_slot04_sel(void) {
    game_state.field_09 = 0;
    game_state.alt_flag = 0;
    game_state.side_mask = 0;
    data_8018f5a0->step_index = 2;
    data_8018f5a0->substep_index = 0;
    data_8018f5a0->step_index_2 = 0;
    data_8018f5a0->pass_index = 0;
    data_8018f5a0->field_54 = 0;
}
