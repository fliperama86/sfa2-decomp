/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014fd28(Ctl *ctl) {
    Stream *stream = (Stream *)ctl;
    int v;
    if (stream->step_count == 0) {
        v = stream->current_record->kind;
        if (v < 4) {
            stream->vab_id = v;
            stream->open_source_ptr = stream->source_ptr;
        }
    }
    func_8015003c(stream);
    stream->step_count++;
}
