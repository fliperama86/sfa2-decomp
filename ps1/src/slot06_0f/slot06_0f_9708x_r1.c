/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9790_slot06_0f(Object *obj);

extern SequenceStep *data_801ecba4_slot06_0f[];

void func_801e9708_slot06_0f(Object *obj) {
    int idx = 2;

    obj->field_0f = 1;
    obj->field_0a = 1;
    obj->field_0c = 0;
    obj->field_81 = 4;
    obj->field_04++;
    obj->pos_y += 0x70;
    if (obj->field_03 != 0) {
        idx = 4;
    }
    func_80130700(obj, data_801ecba4_slot06_0f[idx]);
    func_801e9790_slot06_0f(obj);
}
