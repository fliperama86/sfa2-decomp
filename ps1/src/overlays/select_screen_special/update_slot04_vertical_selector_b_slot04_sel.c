/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 data_801b7de4_slot04_sel[];
extern u8 data_801b8320_slot04_sel[];
extern u8 data_801b8328_slot04_sel[];
extern TextBuf *data_801b8474_slot04_sel[];
extern TextBuf *data_801b8540_slot04_sel[];
extern TextBuf *data_801b85e8_slot04_sel[];
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;
void update_second_menu_cursor_up_down_wrap_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, u16 kind);
void check_opposite_player_same_selection_2_slot04_sel(void);

void update_slot04_vertical_selector_b_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    TextBuf *a = data_801b8540_slot04_sel[obj->side];
    TextBuf *b = data_801b85e8_slot04_sel[obj->side];
    TextBuf *c = data_801b8474_slot04_sel[obj->side];
    if (rec->field_09 == 0 && ((game_state.side_mask >> obj->side) & 1)) {
        data_801b9d30_slot04_sel = rec->saved_index_b;
        data_801b9d34_slot04_sel = rec->field_09;
        update_second_menu_cursor_up_down_wrap_slot04_sel(obj, rec, 1);
        rec->saved_index_b = data_801b9d30_slot04_sel;
        rec->field_09 = data_801b9d34_slot04_sel;
        c->pos_x = data_801b7de4_slot04_sel[obj->side];
        c->pos_y = b->row_height * (data_801b9d30_slot04_sel + 1) + 0x60;
        add_item_to_table(a);
        add_item_to_table(b);
        add_item_to_table(c);
        if (rec->field_09 != 0) {
            data_8016e69a[obj->side] = data_801b9d30_slot04_sel;
            call_func_with_side_x_origin(obj->side, 1);
            b->buf[data_801b8320_slot04_sel[0]] = 0x1b;
            b->buf[data_801b8320_slot04_sel[1]] = 0x1b;
            b->buf[data_801b8320_slot04_sel[data_801b9d30_slot04_sel]] = data_801b8328_slot04_sel[obj->side];
            obj->field_d8 = data_801b9d30_slot04_sel;
            if (obj->field_d8 != 0) {
                obj->field_d4 = (obj->field_d4 & 1) + 4;
                ref_other.p = obj;
                check_opposite_player_same_selection_2_slot04_sel();
                if (scratch_word_a[0] != 0) {
                    obj->field_d4 = obj->field_d4 ^ scratch_word_a[0];
                }
            }
            rec->count++;
        }
    }
}
