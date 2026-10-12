/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 *data_801b7ed4_slot04_sel[];
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;

void set_selection_from_direction_buttons_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, int unused) {
    u8 t;
    data_801b9d30_slot04_sel = 0;
    t = *data_801b7ed4_slot04_sel[obj->side] & 0xf0;
    rec->input_bits = t;
    if (t != 0) {
        data_801b9d34_slot04_sel = 1 << obj->side;
    }
    if (data_801b9d34_slot04_sel == 0) {
        if (*(u16 *)data_801b7ed4_slot04_sel[obj->side] & 0x8000) {
            call_func_with_side_x_origin(obj->side, 0);
            data_801b9d30_slot04_sel = 2;
        }
        if (*(u16 *)data_801b7ed4_slot04_sel[obj->side] & 0x2000) {
            call_func_with_side_x_origin(obj->side, 0);
            data_801b9d30_slot04_sel = 1;
        }
        if (*(u16 *)data_801b7ed4_slot04_sel[obj->side] & 0x1000) {
            call_func_with_side_x_origin(obj->side, 0);
            data_801b9d30_slot04_sel = 4;
        }
        if (*(u16 *)data_801b7ed4_slot04_sel[obj->side] & 0x4000) {
            call_func_with_side_x_origin(obj->side, 0);
            data_801b9d30_slot04_sel = 3;
        }
    }
}
