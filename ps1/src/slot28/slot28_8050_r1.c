/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_8005197c_slot28;
extern Object *data_80051980_slot28[];
extern ObjectRef data_80051984_slot28;
extern ObjectRef data_8005198c_slot28;
extern ObjectRef data_80051990_slot28;
extern Object *data_80051954_slot28[];
extern ObjectRef data_80051958_slot28;
extern u8 data_80032a64_slot28[];
extern u8 data_80032ec8_slot28[];
extern SequenceStep *data_80034518_slot28[];
extern SequenceStep *data_80034514_slot28[];
extern SequenceStep *data_80034528_slot28[];
extern SequenceStep *data_8003455c_slot28[];
extern SequenceStep *data_80034530_slot28[];
void func_8001916c_slot28(void);
void func_80019130_slot28(Object *obj);

void func_80018050_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *b;
    int one = 1;
    h->countdown_timer = 0x258;
    h->step_index_2++;
    func_80151020(0x303);
    func_8001916c_slot28();
    func_801280f0();
    data_80190568 = one;
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80019130_slot28(b);
        data_8005197c_slot28.p = b;
        b->pos_y = -8;
        b->pos_x = 0;
        b->spawn_x = 0;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = one;
        func_80130768(b, 1, data_80034518_slot28);
        b->field_01 = one;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80019130_slot28(b);
        data_80051990_slot28.p = b;
        b->pos_y = -8;
        b->pos_x = 0;
        b->spawn_x = 0;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = one;
        func_80130768(b, 0, data_80034514_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80019130_slot28(b);
        data_80051980_slot28[0] = b;
        b->pos_x = 0x60;
        b->pos_y = 0x20;
        b->field_01 = 0;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80019130_slot28(b);
        data_80051984_slot28.p = b;
        b->pos_x = 0x50;
        b->pos_y = -0x60;
        b->spawn_x = 0x20;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 5;
        func_80130768(b, 0, data_80034528_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        func_80019130_slot28(b);
        data_8005198c_slot28.p = b;
        b->spawn_y = 0x1e0;
        b->pos_x = 0x28;
        b->spawn_x = 0;
        b->image_row = 0;
        b->table_index = one;
        b->ot_depth = 0;
        b->vel_y = 0xb8;
        func_80130768(b, 0, data_8003455c_slot28);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->field_02 = 0x20;
        b->table_index = 6;
        b->field_90 = (void *)0x80060000;
        b->data_ptr_a = data_80032a64_slot28;
        b->data_ptr_b = data_80032ec8_slot28;
        b->pos_x = 0x78;
        b->pos_y = 0xa0;
        b->spawn_x = 0x60;
        b->spawn_y = 0x1e0;
        b->active_count = one;
        b->field_01 = one;
        b->image_row = 0;
        b->ot_depth = 4;
        func_80130768(b, 0, data_80034530_slot28);
        data_80051954_slot28[0] = b;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->field_02 = 0x20;
        b->table_index = 7;
        b->field_90 = (void *)0x80060000;
        b->data_ptr_a = data_80032a64_slot28;
        b->data_ptr_b = data_80032ec8_slot28;
        b->pos_x = 0xf0;
        b->pos_y = 0xa0;
        b->spawn_x = 0x60;
        b->spawn_y = 0x1e0;
        b->active_count = one;
        b->field_01 = one;
        b->image_row = 0;
        b->ot_depth = 4;
        func_80130768(b, 1, data_80034530_slot28);
        data_80051958_slot28.p = b;
    }
    func_8012818c();
}
