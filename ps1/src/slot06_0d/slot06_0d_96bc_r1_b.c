/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea530_slot06_0d[];
extern SequenceStep *data_801eb1b0_slot06_0d[];

Object *func_8011f32c(void);

void func_801e9a20_slot06_0d(Object *obj) {
    data_801ea530_slot06_0d[obj->field_04](obj);
}

void func_801e9a60_slot06_0d(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    Object *c;
    int one = 1;
    o->field_04 = o->field_04 + 1;
    c = func_8011f32c();
    if (c != 0) {
        c->field_02 = 0x19;
        c->field_7a = 0x60;
        c->field_7c = 0x1e0;
        c->field_00 = one;
        c->field_03 = 0;
        c->field_3c = o;
        c->field_81 = 4;
        c->field_0d = 0;
        *(Object **)&o->field_2c = c;
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_03 = 1;
        c->field_00 = 1;
        c->field_02 = 0x19;
        c->field_7a = 0x60;
        c->field_7c = 0x1e0;
        c->field_3c = o;
        c->field_81 = 4;
        c->field_0d = 0;
        obj->field_30 = (s32)c;
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_03 = 2;
        c->field_02 = 0x19;
        c->field_7a = 0x60;
        c->field_7c = 0x1e0;
        c->field_00 = one;
        c->field_3c = o;
        c->field_0d = 0;
        c->field_81 = 4;
        o->field_34 = (u32)c;
    }
    o->field_0a = 1;
    o->field_0f = 1;
    o->field_0d = 0;
    o->field_0c = 0;
    o->field_81 = 4;
    func_80130700(o, data_801eb1b0_slot06_0d[4]);
}
