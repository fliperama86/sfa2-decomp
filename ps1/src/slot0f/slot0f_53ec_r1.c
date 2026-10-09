/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800ebc48_slot0f[];

void func_800e53ec_slot0f(void) {
    Rect rect;
    Job *job = (Job *)func_8011f4a4();
    if (job != 0) {
        rect.x = 0x140;
        rect.y = 0x1ff;
        rect.w = 0x10;
        rect.h = 1;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = data_800ebc48_slot0f;
    }
}
