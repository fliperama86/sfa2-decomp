/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051ac0_slot28;
extern Object *data_80051ac4_slot28[];
extern Object *data_80051a98_slot28[];
extern SequenceStep *data_8003f084_slot28[];
extern SequenceStep *data_8003f090_slot28[];
extern SequenceStep *data_8003f09c_slot28[];
extern u8 data_8003dea0_slot28[];
extern u8 data_8003e230_slot28[];
void func_8001ee74_slot28(Object *obj, int arg);
void func_8001eea4_slot28(Object *obj);
void func_8001ef04_slot28(void);

void func_8001ebb8_slot28(Object *obj) {
    HudState *h;
    Object *p;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_60 = 8;
        h->field_52++;
        func_8001ef04_slot28();
        func_80130768(data_80051ac0_slot28.p, 1, data_8003f084_slot28);
        func_80130768(data_80051ac4_slot28[0], 2, data_8003f090_slot28);
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_8001eea4_slot28(p);
            p->pos_x = 0xc8;
            p->pos_y = 0x90;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 3;
            func_80130768(p, 4, data_8003f09c_slot28);
            data_80051a98_slot28[0] = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x70;
            p->field_03 = 2;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_8003dea0_slot28;
            p->field_9c = data_8003e230_slot28;
            p->pos_x = 0xac;
            p->pos_y = 0xaa;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_01 = 0;
            p->field_0d = 0;
            p->field_09 = 2;
            ((Slot28Obj *)p)->field_6c = data_8003f09c_slot28;
            data_80051a98_slot28[1] = p;
        }
        func_8001ee74_slot28(obj, 3);
        func_80128370();
    }
}
