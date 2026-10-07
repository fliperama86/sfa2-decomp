/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern SequenceStep *data_80036690_slot28[];
extern u8 data_80035430_slot28[];
extern u8 data_800357d8_slot28[];
extern ObjectRef data_8005199c_slot28;
extern ObjectRef data_800519a0_slot28;
extern ObjectRef data_800519d0_slot28;
Block172 *func_8011f1e0(void);
void func_8001a958_slot28(Object *obj, int arg);

void func_80019eb4_slot28(Object *obj) {
    Object *p;
    p = data_800519d0_slot28.p;
    if ((s16)p->field_3a < 0) {
        HudState *h = data_8018f5a0;
        h->field_52++;
        h->field_60 = 0xb4;
        p = data_8005199c_slot28.p;
        p->field_48 = 0xff;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x33;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_80035430_slot28;
            p->field_9c = data_800357d8_slot28;
            p->pos_x = 0x68;
            p->pos_y = 0x58;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 3;
            p->field_03 = 1;
            p->field_01 = 0;
            p->field_0d = 0;
            ((Slot28Obj *)p)->field_6c = data_80036690_slot28 + 6;
            data_800519a0_slot28.p = p;
        }
        func_8001a958_slot28(obj, 1);
        func_8014f4d4(6, 1);
    }
}
