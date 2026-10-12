/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern TextItem *data_801b8978_slot04_sel[];
extern Slot04SelRec8978 data_801b89d4_slot04_sel[];
extern s16 data_801b89d8_slot04_sel;
extern s16 data_801b89da_slot04_sel;
extern Slot04SelRec8c00 *data_801b8c00_slot04_sel[];
extern u16 data_801b9d64_slot04_sel;
void save_player_select_values_2_slot04_sel(void);

void start_timed_step_and_advance_slot04_sel(void) {
    game_state.field_09 = 0xff;
    data_801b9d64_slot04_sel = 0x40;
    data_8018f5a0->step_index_2++;
}

void draw_timed_items_then_advance_slot04_sel(void) {
    TextItem *r;
    Slot04SelRec8c00 *s;
    u16 t;

    r = data_801b8978_slot04_sel[data_801a8067];
    r->x = 0x30;
    r->y = 0x50;
    add_item_to_table(r);
    data_801b89d8_slot04_sel = 0xa0;
    data_801b89da_slot04_sel = 0x68;
    add_item_to_table(data_801b89d4_slot04_sel);
    s = data_801b8c00_slot04_sel[data_801a83fb];
    s->pos_x = 0xc0;
    s->pos_y = 0x80;
    add_item_to_table(s);
    t = data_801b9d64_slot04_sel - 1;
    data_801b9d64_slot04_sel = t;
    if (t == 0) {
        data_8018f5a0->step_index_2++;
    }
}

void reset_select_state_and_set_mode_flags_slot04_sel(void) {
    game_state.field_09 = 0;
    game_state.alt_flag = 0;
    game_state.side_mask = 0;
    save_player_select_values_2_slot04_sel();
    data_8018f5a0->step_index = 2;
    data_8018f5a0->substep_index = 0;
    data_8018f5a0->step_index_2 = 0;
    data_8018f5a0->pass_index = 0;
    data_8018f5a0->field_54 = 0;
    player_left.mode_flag_2 = data_8016e68f[0];
    player_right.mode_flag_2 = data_8016e690[0];
}
