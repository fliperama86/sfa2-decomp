/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2aa4_slot04_sel(u8 *dst, u8 n) {
    u8 buf[2];
    int i;
    for (i = 0; i < 2; i++) {
        buf[i] = n % 10;
        n = n / 10;
    }
    n = buf[1];
    dst[0] = n ? n + 0x30 : 0x20;
    dst[1] = buf[0] + 0x30;
}
