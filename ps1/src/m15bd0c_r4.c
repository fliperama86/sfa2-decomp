/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

#define ITOB(v) ((v) / 10 * 16 + (v) % 10)

/* Frame index to BCD minute/second/frame (CdIntToPos shape). */
u8 *func_8015cec4(int i, u8 *p) {
    int m, s;

    i += 150;
    p[2] = ITOB(i % 75);
    i /= 75;
    m = i / 60;
    s = i % 60;
    p[1] = ITOB(s);
    p[0] = ITOB(m);
    return p;
}
