/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"

void func_8016a3b0(int a);

/* The local r is 16 bits wide: as an int, this function differs from the original in 7 instruction slots. */
void func_8015057c(Stream *stream) {
    s16 r;
    while (stream->field_48 > 0x1000) {
        r = func_8015fe50((u8 *)0x801e0000, 0x1000, stream->field_1a);
        func_801691ac(1);
        stream->field_48 -= 0x1000;
    }
    if (stream->field_48 != 0) {
        r = func_8015fe50((u8 *)0x801e0000, stream->field_48, stream->field_1a);
        func_801691ac(1);
    }
    if (r == stream->field_1a) {
        func_8016a3b0(r);
    }
}
