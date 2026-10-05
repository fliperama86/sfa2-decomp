/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Job *func_8011f4a4(void);

void func_801451ac(u8 flag) {
    Rect rect;
    Job *job;
    s16 i;
    s16 mask;

    if (data_8018db14 == 0) {
        data_8018db14 = 1;
        rect.x = 0xb0;
        rect.y = 0x1ef;
        rect.w = 0x10;
        rect.h = 1;
        func_80158028(&rect, data_80189434);
        for (i = 1; i < 16; i++) {
            if (flag == 0) {
                mask = 0x8000;
                data_80189434[i] = data_80189434[i] | mask;
            } else {
                data_80189434[i] &= 0x7fff;
            }
        }
        job = func_8011f4a4();
        if (job != 0) {
            rect.x = 0xb0;
            rect.y = 0x1fc;
            rect.w = 0x10;
            rect.h = 1;
            job->kind = 1;
            job->mode = 2;
            job->rect = rect;
            job->src = (u8 *)data_80189434;
        }
    }
}
