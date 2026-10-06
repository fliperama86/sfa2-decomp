/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Img data_80024450_slot12[];
Job *func_8011f4a4(void);

void func_80013e24_slot12(int idx) {
    Rect rect;
    Slot12Img *img = &data_80024450_slot12[idx];
    Job *job = func_8011f4a4();
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
