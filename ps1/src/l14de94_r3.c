/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014fd9c(Ctl *ctl) {
    Stream *stream = (Stream *)ctl;
    StreamEntry *e;
    int t;
    e = &stream->entries[stream->write_index];
    if (stream->step_count == 0) {
        if (func_80165884(stream->field_44, stream->vab_id, stream->source_ptr) == -1) {
            stream->error_code = 2;
            stream->field_40 = (StreamRec *)((u8 *)stream->field_40 - 0x40);
            t = stream->record_count;
            stream->record_count = t - 2;
            advance_stream_record(stream);
            close_vab_and_sep(stream->vab_id);
            return;
        }
        func_8016d06c(0);
        e->rect_x = 0xffff;
        stream->transfer_flag = 1;
        stream->transfer_remaining = stream->bytes_remaining;
        data_80190a44[stream->vab_id] = stream->vab_id;
        stream->bytes_20[stream->vab_id] = 1;
    }
    e = &stream->entries[stream->write_index];
    if (stream->bytes_remaining > 0x800) {
        e->byte_count = 0x800;
    } else {
        e->byte_count = stream->bytes_remaining;
    }
    func_80150100(stream);
    e->pending_flag = 1;
    e->handler_index = 4;
    e->field_0e = stream->vab_id;
    if (++stream->write_index == 8) {
        stream->write_index = 1;
    }
    stream->step_count++;
}
