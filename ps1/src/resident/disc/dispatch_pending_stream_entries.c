/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void dispatch_pending_stream_entries(Stream *stream) {
    StreamEntry *e;
    int n = 0;
    while (stream->entries[stream->read_index].pending_flag != 0) {
        e = &stream->entries[stream->read_index];
        data_8017ec60[e->handler_index](stream, e);
        e->pending_flag = 0;
        if (++stream->read_index == 8) {
            stream->read_index = 1;
        }
        n++;
        stream->timeout_count = 0;
    }
    if (n == 0 && stream->state == 2) {
        stream->state = 3;
    }
    if (++stream->timeout_count >= 0xb5 && stream->state == 1) {
        close_slots_and_seek_cd_position(stream);
    }
    if (stream->error_code != 0) {
        close_slots_and_seek_cd_position(stream);
    }
}
