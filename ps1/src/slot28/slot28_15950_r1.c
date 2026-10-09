/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051c64_slot28[];
extern Object *data_80051c90_slot28[];
extern ObjectRef data_80051c94_slot28;
extern SequenceStep *data_8004dce4_slot28[];
extern u8 data_8004c73c_slot28[];
extern u8 data_8004cbc4_slot28[];
extern SequenceStep *data_8004dce8_slot28[];
extern SequenceStep *data_8004dd1c_slot28[];
void func_80025ebc_slot28(void);
void func_80025e2c_slot28(Object *obj, int arg);

void func_80025950_slot28(Object *obj) {
    HudState *h;
    Object *b;
    Object *prev;
    int i;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_60 = 0x258;
        h->field_52++;
        func_80025ebc_slot28();
        b = data_80051c90_slot28[0];
        b->field_09 = 5;
        b->pos_x = 0x58;
        b->field_01 = 0;
        b->pos_y = -0x60;
        b = data_80051c94_slot28.p;
        b->pos_x = 0x50;
        b->pos_y = 0x20;
        b->field_7a = 0x20;
        b->field_7c = 0x1e0;
        b->field_0d = 0;
        b->field_09 = 4;
        b->field_01 = 1;
        func_80130768(b, 0, data_8004dce4_slot28);
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_02 = 0x2d;
            b->field_09 = 3;
            b->field_7a = 0x60;
            b->pos_x = 0xb8;
            b->pos_y = 0xa0;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_8004c73c_slot28;
            b->field_9c = data_8004cbc4_slot28;
            b->field_00 = 1;
            b->field_03 = 1;
            b->field_01 = 1;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->box_tables = (BoxTables *)data_8004dce8_slot28;
            data_80051c64_slot28[1] = b;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_02 = 0x2d;
            b->field_09 = 2;
            b->field_7a = 0x60;
            b->pos_x = -8;
            b->pos_y = 0xa0;
            b->field_90 = (void *)0x80060000;
            b->field_98 = data_8004c73c_slot28;
            b->field_9c = data_8004cbc4_slot28;
            b->field_00 = 1;
            b->field_03 = 0;
            b->field_01 = 1;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
            b->box_tables = (BoxTables *)data_8004dce8_slot28;
            prev = data_80051c64_slot28[1];
            data_80051c64_slot28[0] = b;
            b->field_58 = (s32)data_8004dd1c_slot28;
            b->field_3c = prev;
        }
        for (i = 0; i < 6; i++) {
            b = (Object *)func_8011f1e0();
            if (b != 0) {
                b->field_02 = 0x2d;
                b->field_03 = 3;
                b->field_09 = 4;
                b->field_7a = 0x60;
                b->field_7c = 0x1e0;
                b->field_90 = (void *)0x80060000;
                b->field_98 = data_8004c73c_slot28;
                b->field_9c = data_8004cbc4_slot28;
                b->field_00 = 1;
                b->field_01 = 1;
                b->field_0d = 0;
                b->box_tables = (BoxTables *)data_8004dce8_slot28;
                data_80051c64_slot28[i + 2] = b;
            }
        }
        func_80025e2c_slot28(obj, 2);
        func_80128370();
    }
}
