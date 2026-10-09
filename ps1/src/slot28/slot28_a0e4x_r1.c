/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051998_slot28[];
extern ObjectRef data_8005199c_slot28;
extern ObjectRef data_800519a0_slot28;
extern ObjectRef data_800519a4_slot28;
extern ObjectRef data_800519a8_slot28;
extern ObjectRef data_800519ac_slot28;
extern Object *data_800519c4_slot28[];
extern ObjectRef data_800519c8_slot28;
extern SequenceStep *data_8003669c_slot28[];
extern SequenceStep *data_800366a8_slot28[];
extern u8 data_80035430_slot28[];
extern u8 data_800357d8_slot28[];
void func_8001a9e8_slot28(void);

void func_8001a0e4_slot28(Object *obj) {
    Object *p;
    Object *q;
    int five;
    if (obj->field_f0 == 0) {
        HudState *h = data_8018f5a0;
        h->field_60 = 0xf0;
        h->field_52 = h->field_52 + 1;
        func_8001a9e8_slot28();
        five = 5;
        p = data_800519c4_slot28[0];
        p->pos_y = 0x20;
        p->field_09 = five;
        func_80130768(p, 1, data_8003669c_slot28);
        p = data_800519c8_slot28.p;
        p->field_01 = 0;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x33;
            p->field_03 = 3;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_80035430_slot28;
            p->field_9c = data_800357d8_slot28;
            p->pos_x = 0x70;
            p->pos_y = 0x70;
            p->field_46 = 0x1000;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 4;
            p->field_01 = 0;
            p->field_0d = 0;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_80051998_slot28[0] = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x33;
            p->field_03 = 4;
            p->field_01 = 0;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_80035430_slot28;
            p->field_9c = data_800357d8_slot28;
            p->pos_x = 0x90;
            p->pos_y = 0x60;
            p->field_46 = 0x2000;
            p->field_7c = 0x1e0;
            p->field_7a = 0x60;
            p->field_0d = 0;
            p->field_09 = 4;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_8005199c_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x33;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_80035430_slot28;
            p->field_9c = data_800357d8_slot28;
            p->pos_x = 0x90;
            p->pos_y = 0x70;
            p->field_46 = 0x3000;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 4;
            p->field_03 = five;
            p->field_01 = 0;
            p->field_0d = 0;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_800519a0_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x33;
            p->field_03 = 6;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_80035430_slot28;
            p->field_9c = data_800357d8_slot28;
            p->pos_x = 0xa0;
            p->pos_y = 0x58;
            p->field_46 = 0x4000;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 4;
            p->field_01 = 0;
            p->field_0d = 0;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_800519a4_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        q = p;
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x33;
            p->field_03 = 7;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_80035430_slot28;
            p->field_9c = data_800357d8_slot28;
            p->pos_x = -0xb0;
            p->pos_y = 0x130;
            p->field_46 = 0x4000;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 2;
            p->field_01 = 0;
            p->field_0d = 0;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_800519a8_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x33;
            p->field_03 = 8;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_80035430_slot28;
            p->field_9c = data_800357d8_slot28;
            p->pos_x = 0xa0;
            p->pos_y = 0x58;
            p->field_46 = 0x4000;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 3;
            p->field_01 = 0;
            p->field_0d = 0;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_800519ac_slot28.p = p;
            p->field_28 = (u32)q;
        }
        func_80128370();
    }
}
