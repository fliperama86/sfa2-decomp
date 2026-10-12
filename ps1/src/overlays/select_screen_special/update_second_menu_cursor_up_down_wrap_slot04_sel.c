/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 *data_801b7ec4_slot04_sel[];
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;

void update_second_menu_cursor_up_down_wrap_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, u16 c) {
    if ((*data_801b7ec4_slot04_sel[obj->side] & 0xf0) != 0) {
        data_801b9d34_slot04_sel = 1 << obj->side;
    }
    if (data_801b9d34_slot04_sel == 0) {
        if ((*data_801b7ec4_slot04_sel[obj->side] & 0x1000) != 0) {
            if (data_801b9d30_slot04_sel == 0) {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9d30_slot04_sel = c;
            } else {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9d30_slot04_sel--;
            }
        }
        if ((*data_801b7ec4_slot04_sel[obj->side] & 0x4000) != 0) {
            if (data_801b9d30_slot04_sel == c) {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9d30_slot04_sel = 0;
            } else {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9d30_slot04_sel++;
            }
        }
    }
}
