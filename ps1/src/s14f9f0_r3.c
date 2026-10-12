/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8015003c(Stream *stream) {
    u32 n = stream->bytes_remaining;
    u8 *p = stream->source_ptr;
    u32 words;
    if (n > 0x800) {
        stream->bytes_remaining = n - 0x800;
        func_8015cdbc(stream->source_ptr, 0x200);
        stream->source_ptr = stream->source_ptr + 0x800;
    } else {
        words = (n + 3) >> 2;
        func_8015cdbc(p, words);
        if (words != 0x200) {
            func_8015cdbc((u8 *)0x801e8000 - ((0x200 - words) << 2), 0x200 - words);
        }
        stream->bytes_remaining = 0;
        stream->source_ptr = stream->source_ptr + (words << 2);
    }
    if (stream->step_count == 0) {
        func_80150180(stream, p);
    }
}

void func_80150100(Stream *stream) {
    func_8015cdbc((u8 *)0x801e0000 + (stream->write_index << 11), 0x200);
    if (stream->bytes_remaining > 0x800) {
        stream->bytes_remaining = stream->bytes_remaining - 0x800;
    } else {
        stream->bytes_remaining = 0;
    }
    if (stream->step_count == 0) {
        func_80150180(stream, (u8 *)0x801e0000 + (stream->write_index << 11));
    }
}

void func_80150180(Stream *stream, u8 *p) {
    if (stream->field_34 != *(u32 *)p) {
        stream->error_code = 3;
    }
}

int func_801501a0(Stream *stream) {
    u16 t;
    stream->record_count++;
    if (stream->record_limit < (s8)stream->record_count) {
        return 0;
    }
    stream->field_40 += 1;
    stream->bytes_remaining = stream->field_40->byte_count;
    t = stream->field_40->field_02;
    stream->source_index = t;
    stream->field_34 = stream->field_40->field_08;
    if (stream->source_index != 5) {
        stream->source_ptr = data_8017eb1c[stream->source_index][(u16)stream->field_40->kind];
    }
    stream->step_count = 0;
    return 1;
}

void func_80150244(Stream *stream) {
    stream->state = 0;
    func_8015c990(0);
    while (func_8015cc44(9, 0, 0) == 0) {
    }
    stream->state = 2;
}
