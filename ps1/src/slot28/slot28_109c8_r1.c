/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern SequenceStep *data_80043864_slot28[];
extern u8 data_8004231c_slot28[];
extern u8 data_800426b8_slot28[];
extern Object *data_80051b18_slot28[];

void func_800209c8_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *p;
    int t = h->field_60 - 1;
    h->field_60 = t;
    if ((s16)t == 0) {
        h->field_60 = 0x168;
        h->field_52++;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x2f;
            p->field_09 = 2;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->pos_x = 0xb8;
            p->pos_y = 0xf0;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_8004231c_slot28;
            p->field_9c = data_800426b8_slot28;
            p->field_03 = 0;
            p->field_01 = 1;
            p->field_0d = 0;
            ((Slot28Obj *)p)->field_6c = data_80043864_slot28;
            data_80051b18_slot28[0] = p;
        }
    }
}
