/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_8003a068_slot28[];
extern u8 data_8003a128_slot28[];
extern u16 data_8003940c_slot28[];

void queue_two_rect_upload_jobs_slot28(Object *o) {
    Rect rect;
    Job *job = (Job *)alloc_pooled_b();
    if (job != 0) {
        rect.x = 0x60;
        rect.y = 0x1e6;
        rect.w = 0x10;
        rect.h = 6;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = data_8003a068_slot28;
        job = (Job *)alloc_pooled_b();
        if (job != 0) {
            rect.x = 0x60;
            rect.y = 0x1f1;
            rect.w = 0x10;
            rect.h = 1;
            job->kind = 1;
            job->mode = 2;
            job->rect = rect;
            job->src = data_8003a128_slot28;
        }
    }
}

void queue_rect_upload_job_slot28(Object *o) {
    Rect rect;
    Job *job = (Job *)alloc_pooled_b();
    if (job != 0) {
        rect.x = 0x60;
        rect.y = 0x1e0;
        rect.w = 0x10;
        rect.h = 0x1f;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = (u8 *)data_8003940c_slot28;
    }
}
