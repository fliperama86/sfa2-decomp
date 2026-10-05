/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014fd28(Stream *stream) {
    int v;
    if (stream->field_24 == 0) {
        v = stream->field_40->field_00;
        if (v < 4) {
            stream->field_1a = v;
            stream->field_44 = stream->field_38;
        }
    }
    func_8015003c(stream);
    stream->field_24++;
}
