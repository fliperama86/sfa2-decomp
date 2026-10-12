/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern TextItem *data_801b791c_slot04_sel[];
extern TextItem data_801b7978_slot04_sel;
extern TextItem *data_801b7ba4_slot04_sel[];
extern u16 data_801b9d24_slot04_sel;
void save_player_select_values_slot04_sel(void);

void draw_versus_items_and_wait_countdown_slot04_sel(void) {
    TextItem *p;
    u16 t;
    Object *q;

    p = data_801b791c_slot04_sel[player_left.kind];
    p->x = 0x30;
    p->y = 0x50;
    add_item_to_table(p);
    q = (Object *)&data_801b7978_slot04_sel;
    data_801b7978_slot04_sel.x = 0xa0;
    data_801b7978_slot04_sel.y = 0x68;
    add_item_to_table(q);
    p = data_801b7ba4_slot04_sel[player_right.kind];
    p->x = 0xc0;
    p->y = 0x80;
    add_item_to_table(p);
    t = data_801b9d24_slot04_sel - 1;
    data_801b9d24_slot04_sel = t;
    if (t == 0) {
        data_8018f5a0->step_index_2++;
    }
}

void reset_select_state_and_set_step_index_2_slot04_sel(void) {
    game_state.field_09 = 0;
    game_state.alt_flag = 0;
    game_state.side_mask = 0;
    save_player_select_values_slot04_sel();
    data_8018f5a0->step_index = 2;
    data_8018f5a0->substep_index = 0;
    data_8018f5a0->step_index_2 = 0;
    data_8018f5a0->pass_index = 0;
    data_8018f5a0->field_54 = 0;
}
