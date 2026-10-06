/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8013f04c(u8 *p, s16 mask, int value, s16 limit, u16 bits) {
    if (bits & mask) {
        p[0] = value;
        p[1]++;
        if (p[1] < limit) return 0;
        p[1] = limit;
        return 1;
    }
    p[0] = p[0] - 1;
    if (p[0] == 0) {
        p[0] = value;
        p[1] = 0;
    }
    return 0;
}
