/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051ccc_slot28[];
extern ObjectRef data_80051cac_slot28;
extern ObjectRef data_80051cb0_slot28;
extern ObjectRef data_80051cc0_slot28;
extern SequenceStep *data_800502c8_slot28[];
extern SequenceStep *data_800502e8_slot28[];
Block172 *func_8011f1e0(void);
void func_8002728c_slot28(Object *o);
void func_8002725c_slot28(Object *o, int arg);
void func_8012818c(void);

void func_80026890_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_52++;
        func_80130768(data_80051ccc_slot28[0], 1, data_800502c8_slot28);
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_8002728c_slot28(p);
            p->pos_x = 0x170;
            p->pos_y = 0x70;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 4;
            func_80130768(p, 0, data_800502e8_slot28);
            data_80051cac_slot28.p = p;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->field_01 = 1;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            func_8002728c_slot28(p);
            p->pos_x = 0x170;
            p->pos_y = 0x70;
            p->field_7a = 0x60;
            p->field_7c = 0x1e0;
            p->field_0d = 0;
            p->field_09 = 2;
            func_80130768(p, 1, data_800502e8_slot28);
            data_80051cb0_slot28.p = p;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->field_01 = 1;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xa6;
            p->field_03 = 0xc;
            data_80051cc0_slot28.p = p;
        }
        func_8002725c_slot28(obj, 1);
        func_8012818c();
    }
}
