/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_800f728c_slot0f[16];
extern u16 data_800f752c_slot0f[20][16];
void func_800e53ec_slot0f(void);

void func_800e52e0_slot0f(void) {
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
        job->src = (u8 *)data_800f728c_slot0f;
    }
}

void func_800e5364_slot0f(void) {
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
        job->src = (u8 *)data_800f752c_slot0f;
        func_800e53ec_slot0f();
    }
}
