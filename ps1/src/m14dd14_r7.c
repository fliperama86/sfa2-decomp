/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014fa60(Ctl *ctl) {
    Stream *stream = (Stream *)ctl;
    int v = (u16)stream->current_record->kind;
    u32 n;
    if (v != 0xffff) {
        if (stream->step_count == 0 && v == 2) {
            if (player_left.kind == player_right.kind) {
                n = stream->bytes_remaining;
                if (n > 0x800) {
                    stream->bytes_remaining = n - 0x800;
                } else {
                    stream->bytes_remaining = 0;
                }
                return;
            }
            stream->source_ptr = (u8 *)(0x800fb100 - data_8017d918[player_right.kind]);
        }
        func_8015003c(stream);
    } else {
        n = stream->bytes_remaining;
        if (n > 0x800) {
            stream->bytes_remaining = n - 0x800;
        } else {
            stream->bytes_remaining = 0;
        }
    }
    stream->step_count++;
}

void func_8014fb54(Ctl *ctl) {
    Stream *stream = (Stream *)ctl;
    StreamEntry *e;
    int a;
    func_80150100(stream);
    if (stream->step_count == 0) {
        a = 0;
    } else {
        a = stream->step_count << 5;
    }
    e = &stream->entries[stream->write_index];
    e->pending_flag = 1;
    e->handler_index = 1;
    e->rect_x = stream->current_record->rect_x + a;
    e->rect_y = stream->current_record->rect_y;
    e->rect_h = 0x20;
    if (++stream->write_index == 8) {
        stream->write_index = 1;
    }
    stream->step_count++;
    e->rect_w = 0x20;
    if (stream->bytes_remaining == 0 && (stream->current_record->field_10 & 0x1f) != 0) {
        e->rect_w = 0x10;
    }
}

void func_8014fc4c(Ctl *ctl) {
    Stream *stream = (Stream *)ctl;
    StreamEntry *e;
    u32 v;
    int a, b;
    func_80150100(stream);
    v = stream->step_count;
    if (v == 0) {
        b = 0;
        a = 0;
    } else {
        a = (v >> 4) << 6;
        b = (v & 0xf) << 4;
    }
    e = &stream->entries[stream->write_index];
    e->pending_flag = 1;
    e->handler_index = 2;
    e->rect_x = stream->current_record->rect_x + a;
    e->rect_y = stream->current_record->rect_y + b;
    if (++stream->write_index == 8) {
        stream->write_index = 1;
    }
    stream->step_count++;
}
