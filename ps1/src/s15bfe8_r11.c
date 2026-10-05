/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

#define BCD(x) (((x) >> 4) * 10 + ((x) & 15))

int func_8015cfc8(u8 *p) {
    unsigned m = p[0];
    unsigned s = p[1];
    unsigned f = p[2];

    return (BCD(m) * 60 + BCD(s)) * 75 + BCD(f) - 150;
}
