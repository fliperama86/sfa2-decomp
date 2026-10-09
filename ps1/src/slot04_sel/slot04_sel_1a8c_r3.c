/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b6e94_slot04_sel[];
extern u8 data_801b9ce8_slot04_sel;
extern u8 data_801b9cec_slot04_sel;

void func_801205c4(int a, unsigned c);

void func_801b1d8c_slot04_sel(Object *obj, Slot04SelRec *r, int arg) {
    int t;
    data_801b9ce8_slot04_sel = 0;
    t = *(u8 *)data_801b6e94_slot04_sel[obj->side + 4] & 0xf0;
    r->field_05 = t;
    if (t != 0) {
        data_801b9cec_slot04_sel = 1 << obj->side;
    }
    if (data_801b9cec_slot04_sel == 0) {
        if ((*data_801b6e94_slot04_sel[obj->side + 4] & 0x8000) != 0) {
            func_801205c4(obj->side, 0);
            data_801b9ce8_slot04_sel = 2;
        }
        if ((*data_801b6e94_slot04_sel[obj->side + 4] & 0x2000) != 0) {
            func_801205c4(obj->side, 0);
            data_801b9ce8_slot04_sel = 1;
        }
        if ((*data_801b6e94_slot04_sel[obj->side + 4] & 0x1000) != 0) {
            func_801205c4(obj->side, 0);
            data_801b9ce8_slot04_sel = 4;
        }
        if ((*data_801b6e94_slot04_sel[obj->side + 4] & 0x4000) != 0) {
            func_801205c4(obj->side, 0);
            data_801b9ce8_slot04_sel = 3;
        }
    }
}
