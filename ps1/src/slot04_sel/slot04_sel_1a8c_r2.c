/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b6e94_slot04_sel[];
extern u8 data_801b9ce8_slot04_sel;
extern u8 data_801b9cec_slot04_sel;

void func_801b1c0c_slot04_sel(Object *obj, int b, int arg) {
    if ((*data_801b6e94_slot04_sel[obj->side + 2] & 0xf0) != 0) {
        data_801b9cec_slot04_sel = 1 << obj->side;
    }
    if (data_801b9cec_slot04_sel == 0) {
        if ((*data_801b6e94_slot04_sel[obj->side + 2] & 0x8000) != 0) {
            if (data_801b9ce8_slot04_sel == 0) {
                func_801205c4(obj->side, 0);
                data_801b9ce8_slot04_sel = arg;
            } else {
                func_801205c4(obj->side, 0);
                data_801b9ce8_slot04_sel--;
            }
        }
        if ((*data_801b6e94_slot04_sel[obj->side + 2] & 0x2000) != 0) {
            if (data_801b9ce8_slot04_sel == (u16)arg) {
                func_801205c4(obj->side, 0);
                data_801b9ce8_slot04_sel = 0;
            } else {
                func_801205c4(obj->side, 0);
                data_801b9ce8_slot04_sel = data_801b9ce8_slot04_sel + 1;
            }
        }
    }
}
