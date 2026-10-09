/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801e4748_slot0b[];

void func_801e09f4_slot0b(Object *obj) {
    Rect rect;
    Job *job = (Job *)func_8011f4a4();
    if (job != 0) {
        rect.x = 0x140;
        rect.y = 0x1e0;
        rect.w = 0x10;
        rect.h = 3;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = data_801e4748_slot0b;
    }
}
