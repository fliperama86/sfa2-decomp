/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801e4cec_slot0b[])(Object *);

void func_801e16d0_slot0b(Slot0bCursor *c) {
    Rect rect;
    Job *job;
    job = (Job *)func_8011f4a4();
    if (job != 0) {
        rect.x = 0x140;
        rect.y = 0x1e7;
        rect.w = 0x10;
        rect.h = 1;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = c->cur->src;
    }
}

void func_801e1764_slot0b(Slot0bObj *obj) {
    data_801e4cec_slot0b[obj->field_04]((Object *)obj);
}
