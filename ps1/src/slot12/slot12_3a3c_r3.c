/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_8002b710_slot12[16];
extern u16 data_8002b9b0_slot12[20][16];
void func_80013d80_slot12(void);

void func_80013c74_slot12(void) {
    Rect rect;
    Job *job = (Job *)func_8011f4a4();
    if (job != 0) {
        rect.x = 0x140;
        rect.y = 0x1e0;
        rect.w = 0x10;
        rect.h = 1;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = (u8 *)data_8002b710_slot12;
    }
}

void func_80013cf8_slot12(void) {
    Rect rect;
    Job *job = (Job *)func_8011f4a4();
    if (job != 0) {
        rect.x = 0x140;
        rect.y = 0x1e1;
        rect.w = 0x10;
        rect.h = 0x14;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = (u8 *)data_8002b9b0_slot12;
        func_80013d80_slot12();
    }
}
