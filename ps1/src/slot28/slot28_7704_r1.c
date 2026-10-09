/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

extern HudState *data_8018f5a0;
extern ObjectRef data_80051938_slot28;
extern Object *data_8005193c_slot28[];
extern ObjectRef data_80051918_slot28;
extern ObjectRef data_80051914_slot28;
extern SequenceStep *data_80031c90_slot28[];
extern SequenceStep *data_80031ca0_slot28[];
extern u8 data_800306d4_slot28[];
extern u8 data_80030a4c_slot28[];
void func_800179d4_slot28(void);
void func_80017944_slot28(Object *obj, int arg);
void func_80128370(void);

void func_80017704_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_60 = 0x438;
        data_8018f5a0->field_52++;
        func_800179d4_slot28();
        func_80130768(data_80051938_slot28.p, 1, data_80031c90_slot28);
        p = data_8005193c_slot28[0];
        p->pos_y = -0x50;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xa6;
            p->field_03 = 5;
            p->field_01 = 0;
            data_80051918_slot28.p = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x34;
            p->field_90 = (void *)0x80060000;
            p->field_98 = data_800306d4_slot28;
            p->field_9c = data_80030a4c_slot28;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->box_tables = (BoxTables *)data_80031ca0_slot28;
            p->field_09 = 4;
            p->pos_x = 0x98;
            p->field_03 = 0;
            p->field_01 = 1;
            p->field_0d = 0;
            p->pos_y = 0x20;
            data_80051914_slot28.p = p;
        }
        func_80017944_slot28(obj, 2);
        func_80128370();
    }
}
