/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void clear_bytes(void *p, unsigned int n) {
    unsigned int i;
    u8 *q = p;
    for (i = 0; i < n; i++) {
        *q++ = 0;
    }
}
