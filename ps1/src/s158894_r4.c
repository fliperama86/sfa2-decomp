/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u32 func_80158f64(Rect *r) {
    int t[4];
    u32 cmd;
    if (r == 0) {
        cmd = 0;
    } else {
        t[0] = (r->x & 0xff) >> 3;
        t[2] = ((-r->w) & 0xff) >> 3;
        t[1] = (r->y & 0xff) >> 3;
        t[3] = ((-r->h) & 0xff) >> 3;
        cmd = 0xe2000000 | (t[1] << 15) | (t[0] << 10) | (t[3] << 5) | t[2];
    }
    return cmd;
}
