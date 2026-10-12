/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 data_801b7e60_slot04_sel[];
extern u16 data_801b7e8c_slot04_sel[];
extern u8 data_801b7df4_slot04_sel[];
extern TextBuf *data_801b8474_slot04_sel[];
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;
void set_selection_from_direction_buttons_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, int kind);

void update_slot04_grid_cursor_kind_index_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    TextBuf *s1 = data_801b8474_slot04_sel[obj->side];
    u8 *p;
    if (rec->active_flag == 0) {
        if (((game_state.side_mask >> obj->side) & 1) != 0) {
            data_801b9d34_slot04_sel = rec->active_flag;
            set_selection_from_direction_buttons_slot04_sel(obj, rec, 0x14);
            rec->table_index = data_801b7df4_slot04_sel[rec->table_index * 5 + data_801b9d30_slot04_sel];
            data_801b9d30_slot04_sel = rec->table_index;
            rec->active_flag = data_801b9d34_slot04_sel;
            s1->pos_x = data_801b7e60_slot04_sel[data_801b9d30_slot04_sel] + obj->side * 0x48;
            s1->pos_y = data_801b7e8c_slot04_sel[data_801b9d30_slot04_sel];
            add_item_to_table(s1);
            if (rec->active_flag != 0) {
                p = data_8016e69d;
                *p = data_801b9d30_slot04_sel;
                call_func_with_side_x_origin(obj->side, 1);
                if (*p == 0) {
                    game_state.kind_index = (u8)(game_state.tick_count % 20);
                } else {
                    game_state.kind_index = *p - 1;
                }
            }
        }
    } else {
        add_item_to_table(s1);
    }
}
