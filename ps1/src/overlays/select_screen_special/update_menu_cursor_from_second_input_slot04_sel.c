/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 *data_801b6e94_slot04_sel[];
extern u8 data_801b9ce8_slot04_sel;
extern u8 data_801b9cec_slot04_sel;

void update_menu_cursor_from_second_input_slot04_sel(Object *obj, int b, int arg) {
    if ((*data_801b6e94_slot04_sel[obj->side + 2] & 0xf0) != 0) {
        data_801b9cec_slot04_sel = 1 << obj->side;
    }
    if (data_801b9cec_slot04_sel == 0) {
        if ((*data_801b6e94_slot04_sel[obj->side + 2] & 0x8000) != 0) {
            if (data_801b9ce8_slot04_sel == 0) {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9ce8_slot04_sel = arg;
            } else {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9ce8_slot04_sel--;
            }
        }
        if ((*data_801b6e94_slot04_sel[obj->side + 2] & 0x2000) != 0) {
            if (data_801b9ce8_slot04_sel == (u16)arg) {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9ce8_slot04_sel = 0;
            } else {
                call_func_with_side_x_origin(obj->side, 0);
                data_801b9ce8_slot04_sel = data_801b9ce8_slot04_sel + 1;
            }
        }
    }
}
