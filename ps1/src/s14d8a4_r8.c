/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_8014e5f4(void) {
    u16 *p = data_80189460;
    data_80189460 = p + 1;
    data_8017d3d4[(s16)*p >> 1]();
    return 1;
}
