/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80048540_slot28[];
extern u16 data_80048140_slot28[];
extern u16 data_80047d40_slot28[];

void func_80022d38_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_80048540_slot28[i];
        data_801a27e4_rows[5][i] = data_80048540_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[3][i] = data_80048140_slot28[i];
        data_801a27e4_rows[8][i] = data_80048140_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_80047d40_slot28[i];
        data_801a27e4_rows[7][i] = data_80047d40_slot28[i];
    }
}
