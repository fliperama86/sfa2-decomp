/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_8004aa48_slot28[];
extern u16 data_8004aa68_slot28[];
extern u16 data_8004aa88_slot28[];

void func_80023b0c_slot28(Object *obj) {
    int i;
    for (i = 0; i < 16; i++) {
        data_801a27e4_rows[0][i] = data_8004aa48_slot28[i];
        data_801a27e4_rows[5][i] = data_8004aa48_slot28[i];
    }
    for (i = 0; i < 16; i++) {
        data_801a27e4_rows[0][16 + i] = data_8004aa68_slot28[i];
        data_801a27e4_rows[5][16 + i] = data_8004aa68_slot28[i];
    }
    for (i = 0; i < 16; i++) {
        data_801a27e4_rows[0][48 + i] = data_8004aa88_slot28[i];
        data_801a27e4_rows[5][48 + i] = data_8004aa88_slot28[i];
    }
}
