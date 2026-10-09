/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801376b8(Object *object) {
    u8 *src;
    Rect rect;

    if (data_8018db14 != 0) {
        return;
    }
    data_8018db14 = 1;
    if (object->field_61 != 0xc && object->field_61 != 0x14 && object->field_61 != 0x16) {
        if (object->field_61 == 0xd || object->field_61 == 0x15 || object->field_61 == 0x17) {
            int idx = 2;

            if (game_state.field_1d & 1) {
                idx = 3;
            }
            src = data_801a27e4 + (object->field_0d << 5);
            rect.x = (object->field_02 << 5) + 0x110;
            rect.y = idx | 0x1e0;
            rect.w = 0x10;
            rect.h = 1;
            func_80158028(&rect, src);
            func_80137220(0, 6);
        }
    } else {
        Job *job;

        src = data_801a27e4 + (object->field_0d << 5);
        rect.x = (object->field_02 << 5) + 0x110;
        rect.y = (game_state.field_1d & 1) + 0x1e0;
        rect.w = 0x10;
        rect.h = 1;
        func_80158028(&rect, src);
        job = (Job *)func_8011f4a4();
        if (job == 0) {
            return;
        }
        rect.x = 0x60;
        rect.y = object->field_0d + 0x1e0;
        rect.w = 0x10;
        rect.h = 1;
        job->kind = 1;
        job->mode = 2;
        job->rect = rect;
        job->src = src;
    }
}
