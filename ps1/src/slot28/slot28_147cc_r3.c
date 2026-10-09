/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051c4c_slot28;
extern Object *data_80051c50_slot28[];
extern Object *data_80051c24_slot28[];
extern ObjectRef data_80051c28_slot28;
extern ObjectRef data_80051c2c_slot28;
extern ObjectRef data_80051c30_slot28;
extern ObjectRef data_80051c34_slot28;
extern ObjectRef data_80051c38_slot28;
extern ObjectRef data_80051c3c_slot28;
extern SequenceStep *data_8004bd74_slot28[];
extern SequenceStep *data_8004bd7c_slot28[];
extern SequenceStep *data_8004bd84_slot28[];
extern u8 data_8004ab90_slot28[];
extern u8 data_8004aed8_slot28[];
Block172 *func_8011f1e0(void);
void func_80128370(void);
void func_80025114_slot28(Object *o);
void func_80025174_slot28(void);
void func_800250e4_slot28(Object *o, int arg);

void func_80024cd0_slot28(Object *obj) {
    HudState *h;
    Object *p;
    int one;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_60 = 0x258;
        h->field_52++;
        func_80025174_slot28();
        func_80130768(data_80051c4c_slot28.p, 1, data_8004bd74_slot28);
        p = data_80051c50_slot28[0];
        p->pos_y = 0x20;
        func_80130768(p, 1, data_8004bd7c_slot28);
        one = 1;
        p->field_01 = one;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80025114_slot28(p);
            p->pos_x = 0xb4;
            p->pos_y = 0x2d;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 2;
            func_80130768(p, 2, data_8004bd84_slot28);
            data_80051c24_slot28[0] = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80025114_slot28(p);
            p->pos_x = 0xbb;
            p->pos_y = 0x2f;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 2;
            func_80130768(p, 3, data_8004bd84_slot28);
            data_80051c28_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80025114_slot28(p);
            p->pos_x = 0xc8;
            p->pos_y = 0x3e;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 2;
            func_80130768(p, 4, data_8004bd84_slot28);
            data_80051c2c_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80025114_slot28(p);
            p->pos_x = 0xd9;
            p->pos_y = 0x4e;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 2;
            func_80130768(p, 5, data_8004bd84_slot28);
            data_80051c30_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80025114_slot28(p);
            p->pos_x = 0xf3;
            p->pos_y = 0x63;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 2;
            func_80130768(p, 6, data_8004bd84_slot28);
            data_80051c34_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_80025114_slot28(p);
            p->pos_x = 0xb8;
            p->pos_y = 0xa0;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 2;
            func_80130768(p, 7, data_8004bd84_slot28);
            data_80051c38_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_02 = 0x1c;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_8004ab90_slot28;
            p->field_9c = data_8004aed8_slot28;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 4;
            p->field_00 = one;
            p->field_03 = 0;
            p->field_01 = one;
            p->field_0d = 0;
            p->field_0b = 0;
            ((Slot28Obj *)p)->field_6c = data_8004bd84_slot28;
            data_80051c3c_slot28.p = p;
        }
        func_800250e4_slot28(obj, 3);
        func_80128370();
    }
}
