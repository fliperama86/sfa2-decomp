/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80150294(Stream *stream) {
    StreamEntry *e;
    int n = 0;
    while (stream->entries[stream->field_17].field_08 != 0) {
        e = &stream->entries[stream->field_17];
        data_8017ec60[e->field_0a](stream, e);
        e->field_08 = 0;
        if (++stream->field_17 == 8) {
            stream->field_17 = 1;
        }
        n++;
        stream->field_09 = 0;
    }
    if (n == 0 && stream->field_0a == 2) {
        stream->field_0a = 3;
    }
    if (++stream->field_09 >= 0xb5 && stream->field_0a == 1) {
        func_8014f76c(stream);
    }
    if (stream->field_14 != 0) {
        func_8014f76c(stream);
    }
}
