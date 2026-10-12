/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void move_selection_up_down_slot04_sel(Object *obj, Slot04SelRec9d78 *rec, int n);
void move_selection_left_right_slot04_sel(Object *obj, Slot04SelRec9d78 *rec, int n);
extern u8 data_801b8c91_slot04_sel;
extern u8 data_801b8c92_slot04_sel;
extern u8 data_801b8c93_slot04_sel;
extern u8 data_801b8c94_slot04_sel;
extern u8 data_801b8ddc_slot04_sel[];
extern u8 data_801b8de4_slot04_sel[];
extern u8 data_801b8de8_slot04_sel[];
extern u8 data_801b8dec_slot04_sel[];
extern u8 data_801b8df0_slot04_sel[];
extern u8 data_801b9d70_slot04_sel;
extern u8 data_801b9d74_slot04_sel;
extern Slot04SelRec8eac data_801b8eac_slot04_sel;
extern Slot04SelRec8eac data_801b8edc_slot04_sel;
extern Slot04SelRec8eac data_801b8f00_slot04_sel;
extern Slot04SelRec8eac data_801b8f30_slot04_sel;

void update_slot04_option_menu_cells_slot04_sel(Object *obj, Slot04SelRec9d78 *rec) {
    Slot04SelRec8eac *a = &data_801b8eac_slot04_sel;
    Slot04SelRec8eac *b = &data_801b8edc_slot04_sel;
    Slot04SelRec8eac *c = &data_801b8f00_slot04_sel;
    Slot04SelRec8eac *d = &data_801b8f30_slot04_sel;

    if (rec->field_0e == 0 && ((game_state.side_mask >> obj->side) & 1)) {
        data_801b9d70_slot04_sel = data_801b8c91_slot04_sel;
        data_801b9d74_slot04_sel = rec->field_0e;
        move_selection_up_down_slot04_sel(obj, rec, 6);
        data_801b8c91_slot04_sel = data_801b9d70_slot04_sel;
        rec->field_0e = data_801b9d74_slot04_sel;
        if (data_801b8c91_slot04_sel == 4) {
            data_801b9d70_slot04_sel = data_801b8c92_slot04_sel;
            move_selection_left_right_slot04_sel(obj, rec, 2);
            data_801b8c92_slot04_sel = data_801b9d70_slot04_sel;
        }
        if (data_801b8c91_slot04_sel == 5) {
            data_801b9d70_slot04_sel = data_801b8c93_slot04_sel;
            move_selection_left_right_slot04_sel(obj, rec, 1);
            data_801b8c93_slot04_sel = data_801b9d70_slot04_sel;
            game_state.table_index_3 = data_801b8df0_slot04_sel[data_801b9d70_slot04_sel];
        }
        if (data_801b8c91_slot04_sel == 6) {
            data_801b9d70_slot04_sel = data_801b8c94_slot04_sel;
            move_selection_left_right_slot04_sel(obj, rec, 2);
            data_801b8c94_slot04_sel = data_801b9d70_slot04_sel;
        }
        if (rec->field_0e != 0) {
            call_func_with_side_x_origin(obj->side, 1);
        }
    }
    a->cell_values[data_801b8ddc_slot04_sel[0]] = 0x1a;
    a->cell_values[data_801b8ddc_slot04_sel[1]] = 0x1a;
    a->cell_values[data_801b8ddc_slot04_sel[2]] = 0x1a;
    a->cell_values[data_801b8ddc_slot04_sel[3]] = 0x1a;
    a->cell_values[data_801b8ddc_slot04_sel[4]] = 0x1a;
    a->cell_values[data_801b8ddc_slot04_sel[5]] = 0x1a;
    a->cell_values[data_801b8ddc_slot04_sel[6]] = 0x1a;
    a->cell_values[data_801b8ddc_slot04_sel[data_801b8c91_slot04_sel]] = 0x10;
    b->cell_values[data_801b8de4_slot04_sel[0]] = 0x1b;
    b->cell_values[data_801b8de4_slot04_sel[1]] = 0x1b;
    b->cell_values[data_801b8de4_slot04_sel[2]] = 0x1b;
    b->cell_values[data_801b8de4_slot04_sel[data_801b8c92_slot04_sel]] = 0x1a;
    c->cell_values[data_801b8de8_slot04_sel[0]] = 0x1b;
    c->cell_values[data_801b8de8_slot04_sel[1]] = 0x1b;
    c->cell_values[data_801b8de8_slot04_sel[data_801b8c93_slot04_sel]] = 0x1a;
    d->cell_values[data_801b8dec_slot04_sel[0]] = 0x1b;
    d->cell_values[data_801b8dec_slot04_sel[1]] = 0x1b;
    d->cell_values[data_801b8dec_slot04_sel[2]] = 0x1b;
    d->cell_values[data_801b8dec_slot04_sel[data_801b8c94_slot04_sel]] = 0x1a;
}
