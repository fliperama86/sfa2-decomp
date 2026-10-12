/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051a98_slot28[];
extern ObjectRef data_80051ac0_slot28;
extern Object *data_80051ac4_slot28[];
extern ObjectRef data_80051ac8_slot28;
extern ObjectRef data_80051ad0_slot28;
extern ObjectRef data_80051ad4_slot28;
extern SequenceStep *data_8003f084_slot28[];
extern SequenceStep *data_8003f080_slot28[];
extern SequenceStep *data_8003f090_slot28[];
extern SequenceStep *data_8003f0b8_slot28[];
extern SequenceStep *data_8003f09c_slot28[];
extern u8 data_8003dea0_slot28[];
extern u8 data_8003e230_slot28[];
void func_8001eee0_slot28(void);
void func_8001eea4_slot28(Object *obj);

void func_8001e4c0_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *b;
    int one = 1;
    h->countdown_timer = 0x12c;
    h->step_index_2++;
    func_80151020(0x506);
    func_8001eee0_slot28();
    func_801280f0();
    data_80190568 = one;
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_8001eea4_slot28(b);
        data_80051ac0_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->spawn_x = 0;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = one;
        func_80130768(b, 0, data_8003f084_slot28);
        b->field_01 = one;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_8001eea4_slot28(b);
        data_80051ad4_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->spawn_x = 0;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = one;
        func_80130768(b, 0, data_8003f080_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_8001eea4_slot28(b);
        data_80051ac4_slot28[0] = b;
        b->pos_x = 0x38;
        b->pos_y = -0x60;
        b->spawn_x = 0x10;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 5;
        func_80130768(b, 0, data_8003f090_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_8001eea4_slot28(b);
        data_80051ac8_slot28.p = b;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_8001eea4_slot28(b);
        data_80051ad0_slot28.p = b;
        b->spawn_y = 0x1e0;
        b->pos_x = 0x28;
        b->spawn_x = 0;
        b->image_row = 0;
        b->table_index = one;
        b->vel_y = 0xb8;
        b->ot_depth = 0;
        func_80130768(b, 0, data_8003f0b8_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->field_02 = 0x70;
        b->field_90 = (void *)0x80060000;
        b->data_ptr_a = data_8003dea0_slot28;
        b->data_ptr_b = data_8003e230_slot28;
        b->pos_x = 0xd0;
        b->pos_y = 0xa8;
        b->spawn_x = 0x60;
        b->spawn_y = 0x1e0;
        b->ot_depth = 2;
        b->active_count = one;
        b->table_index = one;
        b->field_01 = 0;
        b->image_row = 0;
        b->box_tables = (BoxTables *)data_8003f09c_slot28;
        data_80051a98_slot28[0] = b;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->field_02 = 0x70;
        b->field_90 = (void *)0x80060000;
        b->data_ptr_a = data_8003dea0_slot28;
        b->data_ptr_b = data_8003e230_slot28;
        b->pos_x = 0xb0;
        b->pos_y = 0xa8;
        b->spawn_x = 0x60;
        b->spawn_y = 0x1e0;
        b->ot_depth = 3;
        b->active_count = one;
        b->table_index = 0;
        b->field_01 = 0;
        b->image_row = 0;
        b->box_tables = (BoxTables *)data_8003f09c_slot28;
        data_80051a98_slot28[1] = b;
    }
    func_8012818c();
}
