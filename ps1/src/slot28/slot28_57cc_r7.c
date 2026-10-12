/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002c7b8_slot28[];
void queue_row_upload_job_from_table_index_slot28(Object *obj, int idx);

void func_80015d3c_slot28(Object *obj) {
    ((u8 *)&obj->countdown)[1]--;
    if (((u8 *)&obj->countdown)[1] == 0) {
        ((u8 *)&obj->countdown)[0] = (((u8 *)&obj->countdown)[0] + 1) & 3;
        ((u8 *)&obj->countdown)[1] = 2;
        queue_row_upload_job_from_table_index_slot28(obj, ((u8 *)&obj->countdown)[0]);
    }
}

void queue_row_upload_job_from_table_index_slot28(Object *obj, int idx) {
    Rect rect;
    Job *job;
    if (game_state.wait_flag == 0) {
        job = (Job *)alloc_pooled_b();
        if (job != 0) {
            rect.x = 0x60;
            rect.y = 0x1e6;
            rect.w = 0x10;
            rect.h = 1;
            job->kind = 1;
            job->mode = 2;
            job->rect = rect;
            job->src = data_8002c7b8_slot28 + ((u8)idx << 5);
        }
    }
}
