/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80017768_slot27(Object *obj) {
    Rect r;

    *(int *)&obj->field_14 = 0x4000000;
    *(s16 *)&obj->sequence = 0x2;
    r.x = 0x260;
    r.y = 0x100;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_800177c0_slot27(Object *obj) {
    Rect r;

    *(int *)&obj->field_14 = 0x4400000;
    *(s16 *)&obj->sequence = 0x802;
    r.x = 0x260;
    r.y = 0x120;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_80017818_slot27(Object *obj) {
    Rect r;

    *(int *)&obj->field_14 = 0x4800000;
    *(s16 *)&obj->sequence = 0x1002;
    r.x = 0x260;
    r.y = 0x140;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}

void func_80017870_slot27(Object *obj) {
    Rect r;

    *(int *)&obj->field_14 = 0x4c00000;
    *(s16 *)&obj->sequence = 0x1802;
    r.x = 0x260;
    r.y = 0x160;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, data_801a3fe4);
}
