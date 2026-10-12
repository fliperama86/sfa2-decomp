/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80024414_slot12[];
void queue_image_job_from_slot0f_table_entry_slot12(int idx);

void func_80013d80_slot12(void) {
    Rect rect;
    Job *job = (Job *)alloc_pooled_b();
    if (job != 0) {
        rect.x = 0x140;
        rect.y = 0x1ff;
        rect.w = 0x10;
        rect.h = 1;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = data_80024414_slot12;
    }
}

void call_image_job_with_index_0_slot12(Object *obj) {
    queue_image_job_from_slot0f_table_entry_slot12(0);
}
