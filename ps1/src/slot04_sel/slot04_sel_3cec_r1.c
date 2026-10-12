/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801b7de4_slot04_sel[];
extern u8 data_801b8324_slot04_sel[];
extern u8 data_801b8328_slot04_sel[];
extern TextBuf *data_801b8474_slot04_sel[];
extern TextBuf *data_801b8540_slot04_sel[];
extern TextBuf *data_801b85e8_slot04_sel[];
extern TextBuf data_801b8618_slot04_sel;
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;
void update_second_menu_cursor_up_down_wrap_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, u16 kind);

void update_slot04_vertical_selector_c_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    TextBuf *s3 = data_801b8540_slot04_sel[obj->side];
    TextBuf *s4 = data_801b85e8_slot04_sel[obj->side];
    TextBuf *s1 = data_801b8474_slot04_sel[obj->side];
    if (rec->field_0b == 0) {
        if (((game_state.side_mask >> obj->side) & 1) == 0) {
            return;
        }
        data_801b9d30_slot04_sel = rec->saved_index_c;
        data_801b9d34_slot04_sel = rec->field_0b;
        update_second_menu_cursor_up_down_wrap_slot04_sel(obj, rec, 1);
        rec->saved_index_c = data_801b9d30_slot04_sel;
        rec->field_0b = data_801b9d34_slot04_sel;
        s1->pos_x = data_801b7de4_slot04_sel[obj->side + 2];
        s1->pos_y = data_801b8618_slot04_sel.row_height * (data_801b9d30_slot04_sel + 1) + 0x90;
        add_item_to_table(s3);
        add_item_to_table(s4);
        add_item_to_table(s1);
        if (rec->field_0b == 0) {
            return;
        }
        data_8016e69c = data_801b9d30_slot04_sel;
        call_func_with_side_x_origin(obj->side, 1);
        data_801b8618_slot04_sel.buf[data_801b8324_slot04_sel[0]] = 0x1b;
        data_801b8618_slot04_sel.buf[data_801b8324_slot04_sel[1]] = 0x1b;
        data_801b8618_slot04_sel.buf[data_801b8324_slot04_sel[data_801b9d30_slot04_sel]] = data_801b8328_slot04_sel[obj->side];
        if (data_801b9d30_slot04_sel != 0) {
            game_state.table_index_3 = (s8)data_8016e68c;
        } else {
            game_state.table_index_3 = 0;
        }
    } else {
        add_item_to_table(s3);
        add_item_to_table(s4);
    }
}
