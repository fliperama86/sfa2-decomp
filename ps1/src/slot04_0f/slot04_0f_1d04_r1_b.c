/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5aec_slot04_0f[];
extern u16 data_801a2944[];
extern u16 data_801a29e4[];

Pooled *func_8011f4a4(void);

void func_801b22ec_slot04_0f(Object *obj);
void func_801b2340_slot04_0f(Object *obj, u16 *src);

void func_801b22ec_slot04_0f(Object *o) {
    if (o->side == 0) {
        func_80136e90();
    } else {
        func_80136ed0();
    }
    func_8013786c(o);
}

void func_801b2340_slot04_0f(Object *o, u16 *src) {
    Job *job;
    u16 *dst;
    u16 *dst2;
    int i;
    Rect r;

    if (game_state.config->field_65 == 0) {
        dst = data_801a2944;
        if (o->side == 0) {
            dst = data_801a29e4;
        }
        i = 0;
        dst2 = dst + 0xa00;
        do {
            *dst++ = *src;
            *dst2++ = *src++;
            i++;
        } while (i < 0x10);
        job = (Job *)func_8011f4a4();
        if (job != 0) {
            r.x = 0x60;
            r.y = o->field_0d + 0x1e0;
            r.w = 0x10;
            r.h = 1;
            job->kind = 1;
            job->mode = 2;
            job->rect = r;
            dst -= 0x10;
            job->src = (u8 *)dst;
            r.x = 0x60;
            r.y = o->field_0d + 0x1e0;
            r.w = 0x10;
            r.h = 1;
        }
    }
}

void func_801b244c_slot04_0f(Object *obj) {
    data_801c5aec_slot04_0f[obj->field_07](obj);
}
