/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80128370(void);

extern HudState *data_8018f5a0;
extern Object *data_80051cd0_slot28[];
extern Object *data_80051ca4_slot28[];
extern SequenceStep *data_800502d4_slot28[];
extern SequenceStep *data_800502e8_slot28[];
extern u8 data_8004ead4_slot28[];
extern u8 data_8004ef00_slot28[];
Block172 *func_8011f1e0(void);
void func_800272ec_slot28(void);
void func_8002725c_slot28(Object *o, int arg);
void func_8002728c_slot28(Object *o);

void func_80026acc_slot28(Object *obj) {
    HudState *h;
    Object *p;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->field_52++;
        func_800272ec_slot28();
        func_8002725c_slot28(obj, 2);
        p = data_80051cd0_slot28[0];
        p->pos_x = 0x38;
        p->pos_y = 0x10;
        p->field_09 = 8;
        p->field_01 = 1;
        func_80130768(p, 2, data_800502d4_slot28);
        p->field_50 = 0x40000;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_8002728c_slot28(p);
            p->pos_x = 0xb8;
            p->pos_y = 0xa0;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 4;
            func_80130768(p, 3, data_800502e8_slot28);
            data_80051ca4_slot28[4] = p;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->field_50 = 0x40000;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_8002728c_slot28(p);
            p->pos_x = 0x108;
            p->pos_y = 0xa0;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 3;
            func_80130768(p, 5, data_800502e8_slot28);
            data_80051ca4_slot28[5] = p;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->field_50 = 0x40000;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_03 = 0;
            p->field_01 = 1;
            p->field_02 = 0x73;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_8004ead4_slot28;
            p->field_9c = data_8004ef00_slot28;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->field_0d = 0;
            p->field_09 = 2;
            data_80051ca4_slot28[0] = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_03 = 1;
            p->field_01 = 1;
            p->field_02 = 0x73;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_8004ead4_slot28;
            p->field_9c = data_8004ef00_slot28;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->field_0d = 0;
            p->field_09 = 2;
            data_80051ca4_slot28[1] = p;
        }
        func_80128370();
    }
}
