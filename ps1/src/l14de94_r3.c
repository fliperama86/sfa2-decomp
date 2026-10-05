/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014fd9c(Stream *stream) {
    StreamEntry *e;
    int t;
    e = &stream->entries[stream->field_16];
    if (stream->field_24 == 0) {
        if (func_80165884(stream->field_44, stream->field_1a, stream->field_38) == -1) {
            stream->field_14 = 2;
            stream->field_40 = (StreamRec *)((u8 *)stream->field_40 - 0x40);
            t = stream->field_15;
            stream->field_15 = t - 2;
            func_801501a0(stream);
            func_801203b4(stream->field_1a);
            return;
        }
        func_8016d06c(0);
        e->field_0c = 0xffff;
        stream->field_1c = 1;
        stream->field_48 = stream->field_30;
        data_80190a44[stream->field_1a] = stream->field_1a;
        stream->bytes_20[stream->field_1a] = 1;
    }
    e = &stream->entries[stream->field_16];
    if (stream->field_30 > 0x800) {
        e->field_04 = 0x800;
    } else {
        e->field_04 = stream->field_30;
    }
    func_80150100(stream);
    e->field_08 = 1;
    e->field_0a = 4;
    e->field_0e = stream->field_1a;
    if (++stream->field_16 == 8) {
        stream->field_16 = 1;
    }
    stream->field_24++;
}
