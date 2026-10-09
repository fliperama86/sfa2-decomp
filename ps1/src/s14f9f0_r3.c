/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8015c990(int a);

void func_8015003c(Stream *stream) {
    u32 n = stream->field_30;
    u8 *p = stream->field_38;
    u32 words;
    if (n > 0x800) {
        stream->field_30 = n - 0x800;
        func_8015cdbc(stream->field_38, 0x200);
        stream->field_38 = stream->field_38 + 0x800;
    } else {
        words = (n + 3) >> 2;
        func_8015cdbc(p, words);
        if (words != 0x200) {
            func_8015cdbc((u8 *)0x801e8000 - ((0x200 - words) << 2), 0x200 - words);
        }
        stream->field_30 = 0;
        stream->field_38 = stream->field_38 + (words << 2);
    }
    if (stream->field_24 == 0) {
        func_80150180(stream, p);
    }
}

void func_80150100(Stream *stream) {
    func_8015cdbc((u8 *)0x801e0000 + (stream->field_16 << 11), 0x200);
    if (stream->field_30 > 0x800) {
        stream->field_30 = stream->field_30 - 0x800;
    } else {
        stream->field_30 = 0;
    }
    if (stream->field_24 == 0) {
        func_80150180(stream, (u8 *)0x801e0000 + (stream->field_16 << 11));
    }
}

void func_80150180(Stream *stream, u8 *p) {
    if (stream->field_34 != *(u32 *)p) {
        stream->field_14 = 3;
    }
}

int func_801501a0(Stream *stream) {
    u16 t;
    stream->field_15++;
    if (stream->field_1e < (s8)stream->field_15) {
        return 0;
    }
    stream->field_40 += 1;
    stream->field_30 = stream->field_40->field_04;
    t = stream->field_40->field_02;
    stream->field_0b = t;
    stream->field_34 = stream->field_40->field_08;
    if (stream->field_0b != 5) {
        stream->field_38 = data_8017eb1c[stream->field_0b][(u16)stream->field_40->field_00];
    }
    stream->field_24 = 0;
    return 1;
}

void func_80150244(Stream *stream) {
    stream->field_0a = 0;
    func_8015c990(0);
    while (func_8015cc44(9, 0, 0) == 0) {
    }
    stream->field_0a = 2;
}
