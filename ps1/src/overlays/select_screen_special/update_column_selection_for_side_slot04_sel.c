/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 data_801b7d98_slot04_sel[];
extern u8 data_801b7d9c_slot04_sel[][8];
extern u8 data_801b7ddc_slot04_sel[];
extern u8 data_801b8328_slot04_sel[];
extern TextBuf *data_801b8540_slot04_sel[];
extern TextBuf *data_801b8570_slot04_sel[];
extern TextBuf *data_801b85e8_slot04_sel[];
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;
void move_selection_index_by_buttons_with_wrap_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, u16 kind);

void update_column_selection_for_side_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    TextBuf *a = data_801b8540_slot04_sel[obj->side];
    TextBuf *b = data_801b8570_slot04_sel[obj->side];
    TextBuf *c = data_801b85e8_slot04_sel[obj->side];
    int i;
    if (rec->field_07 == 0 && ((game_state.side_mask >> obj->side) & 1)) {
        data_801b9d30_slot04_sel = rec->saved_index_a;
        data_801b9d34_slot04_sel = rec->field_07;
        move_selection_index_by_buttons_with_wrap_slot04_sel(obj, rec, 7);
        rec->saved_index_a = data_801b9d30_slot04_sel;
        rec->field_07 = data_801b9d34_slot04_sel;
        b->pos_x = data_801b7d98_slot04_sel[obj->side] + a->stride * data_801b9d30_slot04_sel;
        b->pos_y = 0x48;
        add_item_to_table(b);
        for (i = 0; i < 8; i++) {
            a->buf[i + 5] = data_801b7d9c_slot04_sel[data_801b9d30_slot04_sel][i];
        }
        add_item_to_table(a);
        add_item_to_table(c);
        if (rec->field_07 != 0) {
            obj->table_column = data_801b7ddc_slot04_sel[data_801b9d30_slot04_sel];
            data_8016e698[obj->side] = data_801b9d30_slot04_sel;
            call_func_with_side_x_origin(obj->side, 1);
            for (i = 0; i < 8; i++) {
                a->buf[i + 5] = data_801b7d9c_slot04_sel[data_801b9d30_slot04_sel][i];
            }
            a->buf[4] = data_801b8328_slot04_sel[obj->side];
            rec->count++;
        }
    }
}
