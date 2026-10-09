/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051ca4_slot28[];
extern ObjectRef data_80051ca8_slot28;
extern ObjectRef data_80051cbc_slot28;
extern Object *data_80051ccc_slot28[];
extern Object *data_80051cd0_slot28[];
extern ObjectRef data_80051cdc_slot28;
extern ObjectRef data_80051ce0_slot28;
extern SequenceStep *data_800502c4_slot28[];
extern SequenceStep *data_800502c8_slot28[];
extern SequenceStep *data_800502d4_slot28[];
extern SequenceStep *data_80050304_slot28[];
extern SequenceStep *data_800502e8_slot28[];
void func_800272c8_slot28(void);
void func_8002728c_slot28(Object *o);

void func_80026480_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *p;
    int one;
    h->field_60 = 0xb4;
    h->field_50++;
    func_80151020(0x600);
    one = 1;
    func_800272c8_slot28();
    func_801280f0();
    data_80190568 = one;
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002728c_slot28(p);
        data_80051ce0_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = one;
        func_80130768(p, 0, data_800502c4_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002728c_slot28(p);
        data_80051ccc_slot28[0] = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->field_7a = 0;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = one;
        func_80130768(p, 1, data_800502c8_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002728c_slot28(p);
        data_80051cd0_slot28[0] = p;
        p->pos_x = 0x38;
        p->pos_y = -0x60;
        p->field_7a = 0x10;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 5;
        p->field_01 = 0;
        func_80130768(p, 0, data_800502d4_slot28);
    }
    p = data_80051cd0_slot28[0];
    p->pos_x = 0x38;
    p->pos_y = -0x50;
    p->field_09 = 8;
    p->field_01 = one;
    func_80130768(p, 1, data_800502d4_slot28);
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002728c_slot28(p);
        data_80051cdc_slot28.p = p;
        p->field_7c = 0x1e0;
        p->pos_x = 0x28;
        p->field_7a = 0;
        p->field_0d = 0;
        p->field_03 = one;
        p->field_09 = 0;
        p->field_50 = 0xb8;
        func_80130768(p, 0, data_80050304_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002728c_slot28(p);
        p->pos_x = 0xb8;
        p->pos_y = 0xa0;
        p->field_7a = 0x60;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 4;
        func_80130768(p, 2, data_800502e8_slot28);
        data_80051ca4_slot28[0] = p;
        p->box_tables = (BoxTables *)data_800502e8_slot28;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002728c_slot28(p);
        p->pos_x = 0x108;
        p->pos_y = 0xa0;
        p->field_7a = 0x60;
        p->field_7c = 0x1e0;
        p->field_0d = 0;
        p->field_09 = 5;
        func_80130768(p, 4, data_800502e8_slot28);
        data_80051ca8_slot28.p = p;
        p->box_tables = (BoxTables *)data_800502e8_slot28;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->field_00 = one;
        p->field_02 = 0xa6;
        p->field_03 = 0xd;
        data_80051cbc_slot28.p = p;
    }
    func_8012818c();
}
