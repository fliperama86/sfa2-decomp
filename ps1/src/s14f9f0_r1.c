/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void read_stream_header_record(Ctl *ctl) {
    Stream *stream = (Stream *)ctl;
    u32 *p = (u32 *)0x801e0000;
    stream->bytes_remaining = 0;
    stream->step_count = 1;
    read_stream_sector_to_write_slot(stream);
    stream->records_start = p;
    stream->record_limit = *(u8 *)p;
    stream->current_record = stream->records_start;
    stream->write_index++;
    if (*p != stream->expected_word) {
        stream->error_code = 1;
    }
}
