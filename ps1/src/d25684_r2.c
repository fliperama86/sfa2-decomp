/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"

void func_8016a3b0(int a);

void func_8015057c(Stream *stream) {
    s16 r;
    if (stream->field_48 > 0x1000) {
        do {
            r = func_8015fe50((u8 *)0x801e0000, 0x1000, stream->field_1a);
            func_801691ac(1);
            stream->field_48 = stream->field_48 - 0x1000;
        } while (stream->field_48 > 0x1000);
    }
    if (stream->field_48 != 0) {
        r = func_8015fe50((u8 *)0x801e0000, stream->field_48, stream->field_1a);
        func_801691ac(1);
    }
    if (r == stream->field_1a) {
        func_8016a3b0(r);
    }
}
