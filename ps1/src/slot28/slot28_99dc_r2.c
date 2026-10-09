/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80036690_slot28[];
extern u8 data_80035430_slot28[];
extern u8 data_800357d8_slot28[];
extern Object *data_80051998_slot28[];
extern ObjectRef data_8005199c_slot28;
extern ObjectRef data_800519c0_slot28;
extern Object *data_800519c4_slot28[];
extern ObjectRef data_800519c8_slot28;
extern ObjectRef data_800519d0_slot28;
extern ObjectRef data_800519d4_slot28;
void func_8001a988_slot28(Object *obj);
void func_8001a9c4_slot28(void);

void func_80019ae8_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int one;
    Object *o;
    h->field_60 = 0xb4;
    h->field_50++;
    func_80151020(0x501);
    one = 1;
    func_8001a9c4_slot28();
    func_801280f0();
    data_80190568 = one;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001a988_slot28(o);
        data_800519c0_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = one;
        func_80130768(o, 1, data_80036690_slot28 + 1);
        o->field_01 = one;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001a988_slot28(o);
        data_800519d4_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = one;
        func_80130768(o, 0, data_80036690_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001a988_slot28(o);
        data_800519c4_slot28[0] = o;
        o->pos_x = 0x60;
        o->pos_y = -0x60;
        o->field_7a = 0x10;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 5;
        func_80130768(o, 0, data_80036690_slot28 + 3);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001a988_slot28(o);
        data_800519c8_slot28.p = o;
        o->pos_x = 0x60;
        o->pos_y = 0x20;
        o->field_7a = 0x20;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 4;
        func_80130768(o, 0, data_80036690_slot28 + 5);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_8001a988_slot28(o);
        data_800519d0_slot28.p = o;
        o->field_7c = 0x1e0;
        o->pos_x = 0x28;
        o->field_7a = 0;
        o->field_0d = 0;
        o->field_03 = one;
        o->field_09 = 0;
        o->field_50 = 0xb8;
        func_80130768(o, 0, data_80036690_slot28 + 20);
        o->field_01 = one;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        SequenceStep **t = data_80036690_slot28 + 6;
        func_8001a988_slot28(o);
        o->pos_x = 0xa8;
        o->pos_y = 0x90;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 2;
        func_80130768(o, 0, t);
        data_80051998_slot28[0] = o;
        ((Slot28Obj *)o)->field_6c = t;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_02 = 0x33;
        o->field_90 = (void *)0x80060000;
        o->field_98 = data_80035430_slot28;
        o->field_9c = data_800357d8_slot28;
        o->pos_x = 0xe8;
        o->pos_y = 0x91;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_09 = 3;
        o->field_00 = one;
        o->field_03 = 0;
        o->field_01 = 0;
        o->field_0d = 0;
        data_8005199c_slot28.p = o;
        ((Slot28Obj *)o)->field_6c = data_80036690_slot28 + 6;
    }
    func_8012818c();
}
