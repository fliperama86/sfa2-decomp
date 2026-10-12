/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014f9f0(Ctl *ctl) {
    Stream *stream = (Stream *)ctl;
    u32 *p = (u32 *)0x801e0000;
    stream->bytes_remaining = 0;
    stream->step_count = 1;
    func_80150100(stream);
    stream->field_3c = p;
    stream->record_limit = *(u8 *)p;
    stream->field_40 = stream->field_3c;
    stream->write_index++;
    if (*p != stream->field_34) {
        stream->error_code = 1;
    }
}
