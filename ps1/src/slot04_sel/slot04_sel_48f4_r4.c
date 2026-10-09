/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot04SelRec9d78 data_801b9d78_slot04_sel[];

void func_801b4dd4_slot04_sel(void) {
    Slot04SelRec9d78 *r = data_801b9d78_slot04_sel + 1;
    if (game_state.field_07 & 1) {
        if (data_801b9d78_slot04_sel[0].field_03 == 0x12) {
            data_801b9d78_slot04_sel[0].field_03 = 4;
        }
        if (data_801b9d78_slot04_sel[0].field_03 == 0x13) {
            data_801b9d78_slot04_sel[0].field_03 = 0x11;
        }
        if (data_801b9d78_slot04_sel[0].field_03 == 0x14) {
            data_801b9d78_slot04_sel[0].field_03 = 2;
        }
    }
    if (game_state.field_07 & 2) {
        if (r->field_03 == 0x12) {
            r->field_03 = 4;
        }
        if (r->field_03 == 0x13) {
            r->field_03 = 0x11;
        }
        if (r->field_03 == 0x14) {
            r->field_03 = 2;
        }
    }
    data_8018f5a0->field_50 = data_8018f5a0->field_50 + 1;
}
