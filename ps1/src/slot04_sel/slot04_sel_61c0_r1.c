/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b8e00_slot04_sel[];
extern u8 data_801b9d70_slot04_sel;
extern u8 data_801b9d74_slot04_sel;

void func_801b61c0_slot04_sel(Object *obj, Slot04SelRec9d78 *rec, int sel) {
    if ((*data_801b8e00_slot04_sel[obj->side] & 0xf0) != 0) {
        data_801b9d74_slot04_sel = 1 << obj->side;
    }
    if (data_801b9d74_slot04_sel == 0) {
        if (*data_801b8e00_slot04_sel[obj->side] & 0x1000) {
            if (data_801b9d70_slot04_sel == 0) {
                func_801205c4(obj->side, 0);
                data_801b9d70_slot04_sel = sel;
            } else {
                func_801205c4(obj->side, 0);
                data_801b9d70_slot04_sel--;
            }
        }
        if (*data_801b8e00_slot04_sel[obj->side] & 0x4000) {
            if (data_801b9d70_slot04_sel == (u16)sel) {
                func_801205c4(obj->side, 0);
                data_801b9d70_slot04_sel = 0;
            } else {
                func_801205c4(obj->side, 0);
                data_801b9d70_slot04_sel++;
            }
        }
    }
}
