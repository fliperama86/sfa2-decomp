/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot12Img data_80024450_slot12[];

void queue_image_job_from_slot0f_table_entry_slot12(int idx) {
    Rect rect;
    Slot12Img *img = &data_80024450_slot12[idx];
    Job *job = (Job *)alloc_pooled_b();
    if (job != 0) {
        rect.x = img->rect.x;
        rect.y = img->rect.y;
        rect.w = img->rect.w;
        rect.h = img->rect.h;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = img->src;
    }
}
