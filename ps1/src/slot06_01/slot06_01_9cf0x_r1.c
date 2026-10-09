/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801eb960_slot06_01[];

void func_801e9cf0_slot06_01(Object *obj) {
    int idx = 0x14;
    s16 d;
    s16 e;
    obj->field_0b = 1;
    obj->field_05++;
    e = obj->field_3c->pos_x;
    d = e - obj->pos_x;
    e = d;
    if (d < 0) {
        e = -d;
        obj->field_0b = 0;
    }
    if (e < 0x61) {
        idx = 0x12;
    }
    func_80130700(obj, data_801eb960_slot06_01[idx]);
}
