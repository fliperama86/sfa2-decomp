/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80024414_slot12[];
void func_80013e24_slot12(int idx);

void func_80013d80_slot12(void) {
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
        job->src = data_80024414_slot12;
    }
}

void func_80013e04_slot12(Object *obj) {
    func_80013e24_slot12(0);
}
