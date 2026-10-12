/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void load_stream_image(Stream *stream, StreamEntry *entry) {
    Rect rect;
    rect.x = entry->rect_x;
    rect.y = entry->field_0e;
    rect.w = entry->rect_w;
    rect.h = entry->rect_h;
    func_80157fc4(&rect, (u8 *)0x801e0000 + (stream->read_index << 11));
    func_80157d9c(0);
}

void load_stream_palette_image(Stream *stream, StreamEntry *entry) {
    Rect rect;
    rect.x = entry->rect_x;
    rect.y = entry->field_0e;
    rect.w = 0x40;
    rect.h = 0x10;
    func_80157fc4(&rect, (u8 *)0x801e0000 + (stream->read_index << 11));
    func_80157d9c(0);
}

void transfer_vab_body_chunk(Stream *stream, StreamEntry *entry) {
    int r;
    if (entry->rect_x == 0xffff) {
        entry->rect_x = 0;
    } else {
        func_801691ac(1);
    }
    r = func_8015fe50((u8 *)0x801e0000 + (stream->read_index << 11), entry->byte_count, (s16)entry->field_0e);
    stream->transfer_remaining = stream->transfer_remaining - entry->byte_count;
    if (stream->transfer_remaining == 0) {
        if ((s16)r == entry->field_0e) {
            func_801691ac(1);
            stream->transfer_flag = 0;
        }
    }
    if ((s16)r == -1) {
        stream->error_code = 2;
    }
}
