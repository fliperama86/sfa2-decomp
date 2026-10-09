/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u8 data_801a27e4[];
extern s16 data_801aa4dc[];
Pooled *func_8011f4a4(void);

void func_80137220(u8 a, u8 b) {
    Rect r;
    Job *job;

    job = (Job *)func_8011f4a4();
    if (job) {
        r.x = b << 4;
        r.y = 0x1e0;
        r.w = 0x10;
        r.h = 0x20;
        job->kind = 1;
        job->mode = 2;
        job->rect = r;
        job->src = data_801a27e4 + (a << 10);
    }
}

void func_801372c8(Object *object) {
    Rect r;
    Job *job;

    job = (Job *)func_8011f4a4();
    if (job) {
        r.x = 0x60;
        r.y = object->field_0d + 0x1e0;
        r.w = 0x10;
        r.h = 4;
        job->kind = 1;
        job->mode = 2;
        job->rect = r;
        job->src = data_801a27e4 + (object->field_0d << 5);
    }
}

void func_80137364(u8 a, u8 b, s16 x, s16 y, u8 h) {
    Rect r;

    r.x = x;
    r.y = y;
    r.w = 0x10;
    r.h = h;
    func_80158028(&r, data_801a27e4 + (a << 10) + (b << 5));
    func_80158028(&r, data_801a27e4 + 0x1400 + (a << 10) + (b << 5));
}
