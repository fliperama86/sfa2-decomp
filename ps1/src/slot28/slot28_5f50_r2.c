/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002c838_slot28[];

void func_80015ffc_slot28(Object *obj, SequenceStep *step) {
    int w;
    int t;
    obj->sequence = step;
    t = *(int *)step;
    w = t;
    t = w >> 16;
    obj->field_38 = t;
    obj->field_3a = w;
}

void func_8001601c_slot28(SequenceStep *step) {
    u16 *p;
    Job *job;
    Rect r;
    int idx;
    int last;
    if (data_8018f598 == 0) {
        p = (u16 *)step->field_04;
        do {
            r.x = *p++;
            r.y = *p++;
            idx = *p++;
            last = *p++ & 0x8000;
            job = (Job *)func_8011f4a4();
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
