/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_800519d8_slot28[];
extern ObjectRef data_80051a00_slot28;
extern Object *data_80051a04_slot28[];
extern ObjectRef data_80051a08_slot28;
extern SequenceStep *data_80039280_slot28[];
extern SequenceStep *data_8003929c_slot28[];
extern u8 data_800374ac_slot28[];
extern u8 data_80037a00_slot28[];
Block172 *func_8011f1e0(void);
void func_80128370(void);
void func_8001c430_slot28(Object *obj, int arg);
void func_8001c4c0_slot28(void);

void func_8001bd48_slot28(Object *obj) {
    Object *p;
    Object *first;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_52++;
        data_8018f5a0->field_60 = 0x12c;
        func_8001c4c0_slot28();
        p = data_80051a04_slot28[0];
        p->field_01 = 0;
        p = data_80051a08_slot28.p;
        p->field_01 = 0;
        func_80130768(data_80051a00_slot28.p, 3, data_80039280_slot28);
        p = (Object *)func_8011f1e0();
        first = p;
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x75;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_800374ac_slot28;
            p->field_9c = data_80037a00_slot28;
            p->pos_x = 0xc0;
            p->pos_y = 0x90;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 4;
            p->field_03 = 0;
            p->field_01 = 1;
            p->field_0d = 0;
            p->field_0b = 0;
            p->box_tables = (BoxTables *)data_8003929c_slot28;
            data_800519d8_slot28[0] = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x75;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_800374ac_slot28;
            p->field_9c = data_80037a00_slot28;
            p->pos_x = 0xc2;
            p->pos_y = 0x80;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 3;
            p->field_03 = 1;
            p->field_01 = 1;
            p->field_0d = 0;
            p->field_0b = 0;
            p->box_tables = (BoxTables *)data_8003929c_slot28;
            data_800519d8_slot28[1] = p;
            ((Slot28Obj *)p)->field_28 = first;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x75;
            p->field_03 = 4;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_800374ac_slot28;
            p->field_9c = data_80037a00_slot28;
            p->pos_x = 0x97;
            p->pos_y = 0x59;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 2;
            p->field_46 = 1;
            p->field_01 = 1;
            p->field_0d = 0;
            p->field_0b = 0;
            p->box_tables = (BoxTables *)data_8003929c_slot28;
            data_800519d8_slot28[2] = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x75;
            p->field_03 = 2;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_800374ac_slot28;
            p->field_9c = data_80037a00_slot28;
            p->pos_x = 0xea;
            p->pos_y = 0x57;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_46 = 1;
            p->field_01 = 1;
            p->field_0d = 0;
            p->field_09 = 2;
            p->field_0b = 1;
            p->box_tables = (BoxTables *)data_8003929c_slot28;
            data_800519d8_slot28[3] = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x75;
            p->field_03 = 3;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_800374ac_slot28;
            p->field_9c = data_80037a00_slot28;
            p->pos_x = 0xc5;
            p->pos_y = 0x66;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_09 = 2;
            p->field_46 = 16;
            p->field_01 = 1;
            p->field_0d = 0;
            p->field_0b = 0;
            p->box_tables = (BoxTables *)data_8003929c_slot28;
            data_800519d8_slot28[4] = p;
        }
        func_8001c430_slot28(obj, 4);
        func_80128370();
    }
}
