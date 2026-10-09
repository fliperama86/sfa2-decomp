/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002c7b8_slot28[];
Pooled *func_8011f4a4(void);
void func_80015d94_slot28(Object *obj, int idx);

void func_80015d3c_slot28(Object *obj) {
    ((u8 *)&obj->field_46)[1]--;
    if (((u8 *)&obj->field_46)[1] == 0) {
        ((u8 *)&obj->field_46)[0] = (((u8 *)&obj->field_46)[0] + 1) & 3;
        ((u8 *)&obj->field_46)[1] = 2;
        func_80015d94_slot28(obj, ((u8 *)&obj->field_46)[0]);
    }
}

void func_80015d94_slot28(Object *obj, int idx) {
    Rect rect;
    Job *job;
    if (game_state.field_f0 == 0) {
        job = (Job *)func_8011f4a4();
        if (job != 0) {
            rect.x = 0x60;
            rect.y = 0x1e6;
            rect.w = 0x10;
            rect.h = 1;
            job->kind = 1;
            job->mode = 2;
            job->rect = rect;
            job->src = data_8002c7b8_slot28 + ((u8)idx << 5);
        }
    }
}
