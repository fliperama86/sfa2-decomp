/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966;

int func_80118fc8(void) {
    int n = (data_801a6966 & 0x10f) == 0x10f && (data_801a696a & 0x800) != 0;
    if ((data_801a6972 & 0x10f) == 0x10f) {
        if (data_801a6976 & 0x800) {
            n++;
        }
    }
    return n;
}
