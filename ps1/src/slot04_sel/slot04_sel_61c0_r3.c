/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b8e10_slot04_sel[];
extern u8 data_801b9d70_slot04_sel;
extern u8 data_801b9d74_slot04_sel;
void func_801205c4(int, unsigned);

void func_801b64c0_slot04_sel(Object *obj, Slot04SelRec9d78 *rec, int unused) {
    u8 t;
    data_801b9d70_slot04_sel = 0;
    t = *(u8 *)data_801b8e10_slot04_sel[obj->side] & 0xf0;
    rec->field_05 = t;
    if (t != 0) {
        data_801b9d74_slot04_sel = 1 << obj->side;
    }
    if (data_801b9d74_slot04_sel == 0) {
        if (*data_801b8e10_slot04_sel[obj->side] & 0x8000) {
            func_801205c4(obj->side, 0);
            data_801b9d70_slot04_sel = 2;
        }
        if (*data_801b8e10_slot04_sel[obj->side] & 0x2000) {
            func_801205c4(obj->side, 0);
            data_801b9d70_slot04_sel = 1;
        }
        if (*data_801b8e10_slot04_sel[obj->side] & 0x1000) {
            func_801205c4(obj->side, 0);
            data_801b9d70_slot04_sel = 4;
        }
        if (*data_801b8e10_slot04_sel[obj->side] & 0x4000) {
            func_801205c4(obj->side, 0);
            data_801b9d70_slot04_sel = 3;
        }
    }
}
