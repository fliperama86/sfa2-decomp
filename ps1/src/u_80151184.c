/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: body matches; original frame is 0x10, built is 8. */
/* Form found by automatic permutation search, then cleaned by hand. */
u8 func_80151184(void) {
    /* The unused local reproduces the stack frame of the original, which reserves the
       space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[4];
    s16 *p = &data_80190126;
    u8 *low;

    *p = ((*p + ((unsigned)((*p * 3) << 16) >> 24)) & 0xff) | (((unsigned)((*p * 3) << 16) >> 24) << 8);
    low = (u8 *)p;
    return *low;
}
