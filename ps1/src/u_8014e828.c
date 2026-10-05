/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966[];

void func_8014e828(void) {
    while (1) {
        if ((data_801a6966[0] & 0x100) && (data_801a6966[2] & 0x800)) func_80152ee8();
        func_801192bc(1);
    }
}
