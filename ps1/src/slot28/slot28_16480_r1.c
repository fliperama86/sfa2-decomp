/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

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
    h->countdown_timer = 0xb4;
    h->step_index_2++;
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
        p->spawn_x = 0;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = one;
        func_80130768(p, 0, data_800502c4_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002728c_slot28(p);
        data_80051ccc_slot28[0] = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->spawn_x = 0;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = one;
        func_80130768(p, 1, data_800502c8_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002728c_slot28(p);
        data_80051cd0_slot28[0] = p;
        p->pos_x = 0x38;
        p->pos_y = -0x60;
        p->spawn_x = 0x10;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 5;
        p->field_01 = 0;
        func_80130768(p, 0, data_800502d4_slot28);
    }
    p = data_80051cd0_slot28[0];
    p->pos_x = 0x38;
    p->pos_y = -0x50;
    p->ot_depth = 8;
    p->field_01 = one;
    func_80130768(p, 1, data_800502d4_slot28);
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002728c_slot28(p);
        data_80051cdc_slot28.p = p;
        p->spawn_y = 0x1e0;
        p->pos_x = 0x28;
        p->spawn_x = 0;
        p->image_row = 0;
        p->table_index = one;
        p->ot_depth = 0;
        p->vel_y = 0xb8;
        func_80130768(p, 0, data_80050304_slot28);
        p->field_01 = one;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002728c_slot28(p);
        p->pos_x = 0xb8;
        p->pos_y = 0xa0;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 4;
        func_80130768(p, 2, data_800502e8_slot28);
        data_80051ca4_slot28[0] = p;
        p->box_tables = (BoxTables *)data_800502e8_slot28;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        func_8002728c_slot28(p);
        p->pos_x = 0x108;
        p->pos_y = 0xa0;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 5;
        func_80130768(p, 4, data_800502e8_slot28);
        data_80051ca8_slot28.p = p;
        p->box_tables = (BoxTables *)data_800502e8_slot28;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->active_count = one;
        p->field_02 = 0xa6;
        p->table_index = 0xd;
        data_80051cbc_slot28.p = p;
    }
    func_8012818c();
}
