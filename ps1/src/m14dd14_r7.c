/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014fa60(Stream *stream) {
    int v = (u16)stream->field_40->field_00;
    u32 n;
    if (v != 0xffff) {
        if (stream->field_24 == 0 && v == 2) {
            if (player_left.kind == player_right.kind) {
                n = stream->field_30;
                if (n > 0x800) {
                    stream->field_30 = n - 0x800;
                } else {
                    stream->field_30 = 0;
                }
                return;
            }
            stream->field_38 = (u8 *)(0x800fb100 - data_8017d918[player_right.kind]);
        }
        func_8015003c(stream);
    } else {
        n = stream->field_30;
        if (n > 0x800) {
            stream->field_30 = n - 0x800;
        } else {
            stream->field_30 = 0;
        }
    }
    stream->field_24++;
}

void func_8014fb54(Stream *stream) {
    StreamEntry *e;
    int a;
    func_80150100(stream);
    if (stream->field_24 == 0) {
        a = 0;
    } else {
        a = stream->field_24 << 5;
    }
    e = &stream->entries[stream->field_16];
    e->field_08 = 1;
    e->field_0a = 1;
    e->field_0c = stream->field_40->field_0c + a;
    e->field_0e = stream->field_40->field_0e;
    e->field_12 = 0x20;
    if (++stream->field_16 == 8) {
        stream->field_16 = 1;
    }
    stream->field_24++;
    e->field_10 = 0x20;
    if (stream->field_30 == 0 && (stream->field_40->field_10 & 0x1f) != 0) {
        e->field_10 = 0x10;
    }
}

void func_8014fc4c(Stream *stream) {
    StreamEntry *e;
    u32 v;
    int a, b;
    func_80150100(stream);
    v = stream->field_24;
    if (v == 0) {
        b = 0;
        a = 0;
    } else {
        a = (v >> 4) << 6;
        b = (v & 0xf) << 4;
    }
    e = &stream->entries[stream->field_16];
    e->field_08 = 1;
    e->field_0a = 2;
    e->field_0c = stream->field_40->field_0c + a;
    e->field_0e = stream->field_40->field_0e + b;
    if (++stream->field_16 == 8) {
        stream->field_16 = 1;
    }
    stream->field_24++;
}
