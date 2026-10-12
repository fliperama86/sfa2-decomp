/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 *data_801b8e08_slot04_sel[];
extern u8 data_801b9d70_slot04_sel;
extern u8 data_801b9d74_slot04_sel;

void move_selection_left_right_slot04_sel(Object *obj, Slot04SelRec9d78 *rec, int sel) {
    if ((*data_801b8e08_slot04_sel[obj->side] & 0xf0) != 0) {
        data_801b9d74_slot04_sel = 1 << obj->side;
    }
    if (data_801b9d74_slot04_sel == 0) {
        if (*data_801b8e08_slot04_sel[obj->side] & 0x8000) {
            if (data_801b9d70_slot04_sel == 0) {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9d70_slot04_sel = sel;
            } else {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9d70_slot04_sel--;
            }
        }
        if (*data_801b8e08_slot04_sel[obj->side] & 0x2000) {
            if (data_801b9d70_slot04_sel == (u16)sel) {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9d70_slot04_sel = 0;
            } else {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9d70_slot04_sel++;
            }
        }
    }
}
