/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_8002c838_slot28[];

void set_sequence_step_timer_and_flags_slot28(Object *obj, SequenceStep *step) {
    int w;
    int t;
    obj->sequence = step;
    t = *(int *)step;
    w = t;
    t = w >> 16;
    obj->step_timer = t;
    obj->step_flags = w;
}

void queue_step_row_upload_jobs_slot28(SequenceStep *step) {
    u16 *p;
    Job *job;
    Rect r;
    int idx;
    int last;
    if (white_fill_active == 0) {
        p = (u16 *)step->data_ptr;
        do {
            r.x = *p++;
            r.y = *p++;
            idx = *p++;
            last = *p++ & 0x8000;
            job = (Job *)alloc_pooled_b();
            if (job == 0) {
                break;
            }
            r.w = 0x10;
            r.h = 1;
            job->kind = 1;
            job->mode = 2;
            job->rect = r;
            job->src = data_8002c838_slot28 + ((s16)idx << 5);
        } while (last == 0);
    }
}
