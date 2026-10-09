/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_8002c328_slot28[];
extern u16 data_8002bf28_slot28[];

void func_8001584c_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_8002c328_slot28[i];
        data_801a27e4_rows[5][i] = data_8002c328_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_8002bf28_slot28[i];
        data_801a27e4_rows[7][i] = data_8002bf28_slot28[i];
    }
}
