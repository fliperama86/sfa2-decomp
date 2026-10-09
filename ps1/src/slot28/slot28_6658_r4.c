/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800518d0_slot28[];
extern ObjectRef data_800518f8_slot28;
extern Object *data_800518fc_slot28[];
extern SequenceStep *data_8002fc90_slot28[];
extern SequenceStep *data_8002fca8_slot28[];
extern u8 data_8002e1d4_slot28[];
extern u8 data_8002e758_slot28[];
void func_80016e7c_slot28(void);
void func_80016e28_slot28(Object *obj, int arg);

void func_80016abc_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        HudState *h = data_8018f5a0;
        Object *p;
        int x;
        int one;
        h->field_60 = 0x12c;
        h->field_52++;
        func_80016e7c_slot28();
        x = 0x60;
        p = data_800518fc_slot28[0];
        p->pos_x = x;
        p->pos_y = -0x60;
        func_80130768(data_800518f8_slot28.p, 0, data_8002fc90_slot28);
        p = (Object *)func_8011f1e0();
        one = 1;
        if (p != 0) {
            p->field_02 = 0x72;
            p->field_03 = 2;
            p->field_09 = 4;
            p->field_7c = 0x1e0;
            p->pos_x = 0xf8;
            p->pos_y = 0x90;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_8002e1d4_slot28;
            p->field_00 = one;
            p->field_01 = one;
            p->field_7a = x;
            p->field_0d = 0;
            p->field_9c = data_8002e758_slot28;
            if ((((Slot28Obj *)obj)->field_70 & 0x7f) == 4) {
                func_80130768(p, 4, data_8002fca8_slot28);
            } else {
                func_80130768(p, 0xa, data_8002fca8_slot28);
            }
            data_800518d0_slot28[0] = p;
        }
        p = (Object *)func_8011f1e0();
        one = 1;
        if (p != 0) {
            p->field_02 = 0x72;
            p->field_03 = 3;
            p->field_09 = 4;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->pos_x = 0xa0;
            p->pos_y = 0x90;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_8002e1d4_slot28;
            p->field_00 = one;
            p->field_01 = one;
            p->field_0d = 0;
            p->field_9c = data_8002e758_slot28;
            if ((((Slot28Obj *)obj)->field_70 & 0x7f) == 4) {
                func_80130768(p, 6, data_8002fca8_slot28);
            } else {
                func_80130768(p, 0xb, data_8002fca8_slot28);
            }
            data_800518d0_slot28[1] = p;
        }
        func_80016e28_slot28(obj, 4);
        func_80128370();
    }
}
