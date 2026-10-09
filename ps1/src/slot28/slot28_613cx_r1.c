/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern SequenceStep *data_8002fc8c_slot28[];
extern SequenceStep *data_8002fc90_slot28[];
extern SequenceStep *data_8002fca0_slot28[];
extern SequenceStep *data_8002fca8_slot28[];
extern SequenceStep *data_8002fce8_slot28[];
extern u8 data_8002e1d4_slot28[];
extern u8 data_8002e758_slot28[];
extern ObjectRef data_800518f8_slot28;
extern Object *data_800518fc_slot28[];
extern ObjectRef data_80051908_slot28;
extern Object *data_8005190c_slot28[];
extern Object *data_800518d0_slot28[];
extern Object *data_800518d4_slot28[];
extern Object *data_800518d8_slot28[];
extern Object *data_800518dc_slot28[];
Block172 *func_8011f1e0(void);
void func_801280f0(void);
void func_8012818c(void);
void func_80016540_slot28(Object *obj);
void func_80016e58_slot28(void);

void func_8001613c_slot28(Object *obj) {
    Object *o;
    int one;
    data_8018f5a0->field_60 = 0x258;
    data_8018f5a0->field_50 = data_8018f5a0->field_50 + 1;
    func_80016e58_slot28();
    func_801280f0();
    one = 1;
    data_80190568 = one;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80016540_slot28(o);
        data_800518f8_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = one;
        func_80130768(o, 2, data_8002fc90_slot28);
        o->field_01 = one;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80016540_slot28(o);
        data_8005190c_slot28[0] = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = one;
        func_80130768(o, 0, data_8002fc8c_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80016540_slot28(o);
        data_800518fc_slot28[0] = o;
        o->pos_x = 0x40;
        o->pos_y = 0x20;
        o->field_7a = 0x10;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 5;
        func_80130768(o, 0, data_8002fca0_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80016540_slot28(o);
        data_80051908_slot28.p = o;
        o->field_7c = 0x1e0;
        o->pos_x = 0x28;
        o->field_7a = 0;
        o->field_0d = 0;
        o->field_03 = one;
        o->field_50 = 0xb8;
        o->field_09 = 0;
        func_80130768(o, 0, data_8002fce8_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        Slot28Obj *p = (Slot28Obj *)obj;
        o->field_02 = 0x72;
        o->field_03 = 4;
        o->field_09 = 2;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->pos_x = 0xf8;
        o->pos_y = 0xa0;
        o->field_90 = (void *)0x80060000;
        o->field_98 = data_8002e1d4_slot28;
        o->field_00 = one;
        o->field_01 = one;
        o->field_0d = 0;
        o->field_9c = data_8002e758_slot28;
        if ((p->field_70 & 0x7f) == 4) {
            func_80130768(o, 0, data_8002fca8_slot28);
        } else {
            func_80130768(o, 7, data_8002fca8_slot28);
        }
        data_800518d0_slot28[0] = o;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x72;
        o->field_09 = 3;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->pos_x = 0x88;
        o->pos_y = 0x80;
        o->field_50 = 0x8000;
        o->field_58 = -0x400;
        o->field_90 = (void *)0x80060000;
        o->field_98 = data_8002e1d4_slot28;
        o->field_03 = 0;
        o->field_01 = 1;
        o->field_0d = 0;
        o->field_9c = data_8002e758_slot28;
        func_80130768(o, 1, data_8002fca8_slot28);
        data_800518d4_slot28[0] = o;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x72;
        o->field_09 = 4;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->pos_x = 0x90;
        o->pos_y = 0x78;
        o->field_50 = 0xc000;
        o->field_58 = -0x800;
        o->field_90 = (void *)0x80060000;
        o->field_98 = data_8002e1d4_slot28;
        o->field_03 = 1;
        o->field_01 = 1;
        o->field_0d = 0;
        o->field_9c = data_8002e758_slot28;
        func_80130768(o, 5, data_8002fca8_slot28);
        data_800518d8_slot28[0] = o;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0xa6;
        o->field_03 = 1;
        data_800518dc_slot28[0] = o;
    }
    func_80151020(0x304);
    func_8012818c();
}
