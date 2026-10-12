/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5aec_slot04_0f[];
extern u16 data_801a2944[];
extern u16 data_801a29e4[];

Pooled *alloc_pooled_b(void);

void call_side_routine_and_set_side_flag_slot04_0f(Object *obj);
void queue_16_word_row_upload_job_slot04_0f(Object *obj, u16 *src);

void call_side_routine_and_set_side_flag_slot04_0f(Object *o) {
    if (o->side == 0) {
        func_80136e90();
    } else {
        func_80136ed0();
    }
    set_side_flag(o);
}

void queue_16_word_row_upload_job_slot04_0f(Object *o, u16 *src) {
    Job *job;
    u16 *dst;
    u16 *dst2;
    int i;
    Rect r;

    if (game_state.config->pause_flag_a == 0) {
        dst = data_801a2944;
        if (o->side == 0) {
            dst = data_801a29e4;
        }
        i = 0;
        dst2 = dst + 0xa00;
        do {
            *dst++ = *src;
            *dst2++ = *src++;
            i++;
        } while (i < 0x10);
        job = (Job *)alloc_pooled_b();
        if (job != 0) {
            r.x = 0x60;
            r.y = o->image_row + 0x1e0;
            r.w = 0x10;
            r.h = 1;
            job->kind = 1;
            job->mode = 2;
            job->rect = r;
            dst -= 0x10;
            job->src = (u8 *)dst;
            r.x = 0x60;
            r.y = o->image_row + 0x1e0;
            r.w = 0x10;
            r.h = 1;
        }
    }
}

void call_table_function_by_state_3_7_slot04_0f(Object *obj) {
    data_801c5aec_slot04_0f[obj->state_3](obj);
}
