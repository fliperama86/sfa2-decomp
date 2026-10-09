/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8003a068_slot28[];
extern u8 data_8003a128_slot28[];
extern u16 data_8003940c_slot28[];

void func_8001c2b4_slot28(Object *o) {
    Rect rect;
    Job *job = (Job *)func_8011f4a4();
    if (job != 0) {
        rect.x = 0x60;
        rect.y = 0x1e6;
        rect.w = 0x10;
        rect.h = 6;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = data_8003a068_slot28;
        job = (Job *)func_8011f4a4();
        if (job != 0) {
            rect.x = 0x60;
            rect.y = 0x1f1;
            rect.w = 0x10;
            rect.h = 1;
            job->kind = 1;
            job->mode = 2;
            job->rect = rect;
            job->src = data_8003a128_slot28;
        }
    }
}

void func_8001c3ac_slot28(Object *o) {
    Rect rect;
    Job *job = (Job *)func_8011f4a4();
    if (job != 0) {
        rect.x = 0x60;
        rect.y = 0x1e0;
        rect.w = 0x10;
        rect.h = 0x1f;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = (u8 *)data_8003940c_slot28;
    }
}
