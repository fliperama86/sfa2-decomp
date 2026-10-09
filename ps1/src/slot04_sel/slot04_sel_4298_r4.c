/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b7ecc_slot04_sel[];
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;

void func_801b4774_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, u16 c) {
    if ((*data_801b7ecc_slot04_sel[obj->side] & 0xf0) != 0) {
        data_801b9d34_slot04_sel = 1 << obj->side;
    }
    if (data_801b9d34_slot04_sel == 0) {
        if ((*data_801b7ecc_slot04_sel[obj->side] & 0x8000) != 0) {
            if (data_801b9d30_slot04_sel == 0) {
                func_801205c4(obj->side, 0);
                data_801b9d30_slot04_sel = c;
            } else {
                func_801205c4(obj->side, 0);
                data_801b9d30_slot04_sel--;
            }
        }
        if ((*data_801b7ecc_slot04_sel[obj->side] & 0x2000) != 0) {
            if (data_801b9d30_slot04_sel == c) {
                func_801205c4(obj->side, 0);
                data_801b9d30_slot04_sel = 0;
            } else {
                func_801205c4(obj->side, 0);
                data_801b9d30_slot04_sel++;
            }
        }
    }
}
