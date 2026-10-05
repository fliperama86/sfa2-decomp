/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014f9f0(Stream *stream) {
    u32 *p = (u32 *)0x801e0000;
    stream->field_30 = 0;
    stream->field_24 = 1;
    func_80150100(stream);
    stream->field_3c = p;
    stream->field_1e = *(u8 *)p;
    stream->field_40 = stream->field_3c;
    stream->field_16++;
    if (*p != stream->field_34) {
        stream->field_14 = 1;
    }
}
