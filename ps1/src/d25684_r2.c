/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"

/* The local r is 16 bits wide: as an int, this function differs from the original in 7 instruction slots. */
void transfer_vab_body(Stream *stream) {
    s16 r;
    while (stream->transfer_remaining > 0x1000) {
        r = func_8015fe50((u8 *)0x801e0000, 0x1000, stream->vab_id);
        func_801691ac(1);
        stream->transfer_remaining -= 0x1000;
    }
    if (stream->transfer_remaining != 0) {
        r = func_8015fe50((u8 *)0x801e0000, stream->transfer_remaining, stream->vab_id);
        func_801691ac(1);
    }
    if (r == stream->vab_id) {
        func_8016a3b0(r);
    }
}
