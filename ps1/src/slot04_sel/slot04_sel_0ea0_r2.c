/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void check_opposite_player_same_selection_slot04_sel(void);
void update_selection_index_wrap_by_side_buttons_slot04_sel(Object *obj, Slot04SelRec *rec, int a);

extern TextBuf *data_801b7618_slot04_sel[];
extern TextItem *data_801b749c_slot04_sel[];
extern Object *data_801b7504_slot04_sel[];
extern Object *data_801b6f64_slot04_sel[];
extern u16 data_801b6e7c_slot04_sel[];
extern u8 data_801b7348_slot04_sel[];
extern u8 data_801b7350_slot04_sel[];
extern u8 data_801b9ce8_slot04_sel;
extern u8 data_801b9cec_slot04_sel;

void update_side_selection_menu_cursor_and_confirm_slot04_sel(Object *obj, Slot04SelRec *rec) {
    TextBuf *s3 = data_801b7618_slot04_sel[obj->side];
    TextItem *s2 = data_801b749c_slot04_sel[obj->side];

    if (rec->confirm_mask == 0) {
        if ((game_state.side_mask >> obj->side) & 1) {
            data_801b9ce8_slot04_sel = rec->cursor_index;
            data_801b9cec_slot04_sel = rec->confirm_mask;
            update_selection_index_wrap_by_side_buttons_slot04_sel(obj, rec, 1);
            rec->cursor_index = data_801b9ce8_slot04_sel;
            rec->confirm_mask = data_801b9cec_slot04_sel;
            s2->x = data_801b6e7c_slot04_sel[obj->side];
            s2->y = s3->row_height * (data_801b9ce8_slot04_sel + 1) + 0x60;
            add_item_to_table(s3);
            add_item_to_table(s2);
            add_item_to_table(data_801b7504_slot04_sel[obj->side]);
            if (rec->confirm_mask != 0) {
                data_8016e694[obj->side] = data_801b9ce8_slot04_sel;
                call_func_with_side_x_origin(obj->side, 1);
                s3->buf[data_801b7348_slot04_sel[0]] = 0x1b;
                s3->buf[data_801b7348_slot04_sel[1]] = 0x1b;
                s3->buf[data_801b7348_slot04_sel[data_801b9ce8_slot04_sel]] = data_801b7350_slot04_sel[obj->side];
                obj->field_d8 = data_801b9ce8_slot04_sel;
                if (obj->field_d8 != 0) {
                    obj->field_d4 = (obj->field_d4 & 1) + 4;
                    ref_other.p = obj;
                    check_opposite_player_same_selection_slot04_sel();
                    if (data_8019045c[0] != 0) {
                        obj->field_d4 = obj->field_d4 ^ data_8019045c[0];
                    }
                }
                rec->state = rec->state + 1;
            }
        } else if (((game_state.mode >> obj->side) & 1) == 0) {
            add_item_to_table(data_801b6f64_slot04_sel[obj->side]);
        } else {
            add_item_to_table(s3);
            add_item_to_table(data_801b7504_slot04_sel[obj->side]);
        }
    } else {
        rec->state = 2;
    }
}
