/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern TextBuf *data_801b7618_slot04_sel[];
extern TextItem *data_801b749c_slot04_sel[];
extern Object *data_801b7504_slot04_sel[];
extern Object *data_801b6f64_slot04_sel[];
extern u16 data_801b6e80_slot04_sel[];
extern u8 data_801b734c_slot04_sel[];
extern u8 data_801b7350_slot04_sel[];
extern Slot04SelRec7648 data_801b7648_slot04_sel;
extern u8 data_801b9ce8_slot04_sel;
extern u8 data_801b9cec_slot04_sel;
void update_selection_index_wrap_by_side_buttons_slot04_sel(Object *obj, Slot04SelRec *r, int c);

void update_side_second_menu_cursor_and_confirm_slot04_sel(Object *obj, Slot04SelRec *r) {
    TextBuf *s3 = data_801b7618_slot04_sel[obj->side];
    TextItem *s2 = data_801b749c_slot04_sel[obj->side];

    if (r->field_0b == 0) {
        if (((game_state.side_mask >> obj->side) & 1) != 0) {
            data_801b9ce8_slot04_sel = r->second_cursor_index;
            data_801b9cec_slot04_sel = r->field_0b;
            update_selection_index_wrap_by_side_buttons_slot04_sel(obj, r, 1);
            r->second_cursor_index = data_801b9ce8_slot04_sel;
            r->field_0b = data_801b9cec_slot04_sel;
            s2->x = data_801b6e80_slot04_sel[obj->side];
            s2->y = data_801b7648_slot04_sel.row_height * (data_801b9ce8_slot04_sel + 1) + 0x90;
            add_item_to_table(s3);
            add_item_to_table(s2);
            add_item_to_table(data_801b7504_slot04_sel[obj->side]);
            if (r->field_0b != 0) {
                data_8016e696 = data_801b9ce8_slot04_sel;
                call_func_with_side_x_origin(obj->side, 1);
                data_801b7648_slot04_sel.text_buf[data_801b734c_slot04_sel[0]] = 0x1b;
                data_801b7648_slot04_sel.text_buf[data_801b734c_slot04_sel[1]] = 0x1b;
                data_801b7648_slot04_sel.text_buf[data_801b734c_slot04_sel[data_801b9ce8_slot04_sel]] = data_801b7350_slot04_sel[obj->side];
                if (data_801b9ce8_slot04_sel != 0) {
                    game_state.table_index_3 = (s8)data_8016e68c;
                } else {
                    game_state.table_index_3 = 0;
                }
            }
        } else if (((game_state.mode >> obj->side) & 1) == 0) {
            add_item_to_table(data_801b6f64_slot04_sel[obj->side]);
        } else {
            goto both;
        }
    } else {
both:
        add_item_to_table(s3);
        add_item_to_table(data_801b7504_slot04_sel[obj->side]);
    }
}
