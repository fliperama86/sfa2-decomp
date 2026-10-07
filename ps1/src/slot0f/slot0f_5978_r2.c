/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Img data_800ebc94_slot0f[];
extern HudState *data_8018f5a0;
Pooled *func_8011f4a4(void);

void func_800e5a9c_slot0f(Slot0fObj *obj, int flag) {
    if (flag == 0) {
        data_8018f5a0->field_4a++;
    }
    data_8018f5a0->field_4a = 0;
    data_8018f5a0->field_4c = 0;
    data_8018f5a0->field_4e = 0;
    obj->field_ee = 0;
    obj->field_f0 = 0;
}

void func_800e5ae4_slot0f(Slot0fObj *obj) {
    HudState *h = data_8018f5a0;
    h->field_48++;
    h->field_4a = 0;
    h->field_4c = 0;
    h->field_4e = 0;
    obj->field_ee = 0;
    obj->field_f0 = 0;
    func_8011eb14();
    func_8011eae4();
}

void func_800e5b30_slot0f(int idx) {
    Rect rect;
    Slot12Img *img = &data_800ebc94_slot0f[idx];
    Job *job = (Job *)func_8011f4a4();
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
