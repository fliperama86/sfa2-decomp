/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern SequenceStep *data_8004384c_slot28[];
extern SequenceStep *data_80043858_slot28[];
extern SequenceStep *data_80043864_slot28[];
extern u8 data_8004231c_slot28[];
extern u8 data_800426b8_slot28[];
extern Object *data_80051b18_slot28[];
extern ObjectRef data_80051b40_slot28;
extern Object *data_80051b44_slot28[];
Block172 *func_8011f1e0(void);
void func_800210f0_slot28(void);
void func_80021060_slot28(Object *obj, int arg);
void func_80128370(void);

void func_80020c38_slot28(Object *obj) {
    Object *p;
    int one;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x1a4;
        data_8018f5a0->field_52++;
        func_800210f0_slot28();
        func_80130768(data_80051b40_slot28.p, 0, data_8004384c_slot28);
        p = data_80051b44_slot28[0];
        p->pos_y = -0x60;
        one = 1;
        p->field_01 = one;
        func_80130768(p, 0, data_80043858_slot28);
        func_80021060_slot28(obj, 2);
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = one;
            p->field_02 = 0x2f;
            p->field_09 = 2;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->pos_x = 0xb8;
            p->pos_y = 0x90;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_8004231c_slot28;
            p->field_9c = data_800426b8_slot28;
            p->field_03 = one;
            p->field_01 = one;
            p->field_0d = 0;
            ((Slot28Obj *)p)->field_6c = data_80043864_slot28;
            data_80051b18_slot28[0] = p;
        }
        func_80128370();
    }
}
