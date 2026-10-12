/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void store_image_rows_by_mode_and_queue_job(Object *object, u8 mode) {
    Rect rect;
    u8 *src;
    Job *job;

    src = data_801a27e4 + ((object->image_row + 3) << 5);
    rect.x = (object->field_02 << 5) + 0x110;
    if (mode == 0) {
        rect.y = 0x1e4;
    } else if (mode == 1) {
        rect.y = (object->field_d4 << 1) + 0x1f0;
    } else if (mode == 2) {
        rect.y = (object->field_d4 << 2) + 0x1e9;
    }
    rect.w = 0x10;
    rect.h = 2;
    func_80158028(&rect, src);
    src = data_801a27e4 + 0x1400 + ((object->image_row + 3) << 5);
    func_80158028(&rect, src);
    job = (Job *)alloc_pooled_b();
    if (job != 0) {
        rect.x = 0x60;
        rect.y = object->image_row + 0x1e3;
        rect.w = 0x10;
        rect.h = 2;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = src;
    }
}
