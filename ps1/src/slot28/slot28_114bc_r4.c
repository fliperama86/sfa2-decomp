/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051b8c_slot28;
extern Object *data_80051b90_slot28[];
extern ObjectRef data_80051b9c_slot28;
extern Object *data_80051b64_slot28[];
extern SequenceStep *data_80045c40_slot28[];
extern SequenceStep *data_80045c4c_slot28[];
extern u8 data_80044620_slot28[];
extern u8 data_800449d0_slot28[];
extern SequenceStep *data_80045c58_slot28[];
void func_80021d64_slot28(Object *obj, int arg);
void func_80021df4_slot28(void);

void func_80021afc_slot28(Object *obj) {
    Object *b;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_80021df4_slot28();
        func_80130768(data_80051b8c_slot28.p, 0, data_80045c40_slot28);
        b = data_80051b90_slot28[0];
        b->pos_y = -0x50;
        func_80130768(b, 1, data_80045c4c_slot28);
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x7a;
            b->field_09 = 2;
            b->field_7a = 0x60;
            b->field_7c = 0x1e0;
            b->pos_x = 0xb8;
            b->pos_y = 0x90;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_80044620_slot28;
            b->field_9c = data_800449d0_slot28;
            b->field_03 = 1;
            b->field_01 = 1;
            b->field_0d = 0;
            b->box_tables = (BoxTables *)data_80045c58_slot28;
            data_80051b64_slot28[0] = b;
        }
        func_80021d64_slot28(obj, 3);
        func_80128370();
    }
}

void func_80021c24_slot28(Object *obj) {
    Object *p = data_80051b9c_slot28.p;
    if ((s16)p->field_3a < 0) {
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        p = data_80051b64_slot28[0];
        p->field_48 = 0xff;
        func_80021d64_slot28(obj, 4);
    }
}
