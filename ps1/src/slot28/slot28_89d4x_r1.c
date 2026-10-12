/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_8005197c_slot28;
extern Object *data_80051980_slot28[];
extern ObjectRef data_80051984_slot28;
extern ObjectRef data_80051994_slot28;
extern Object *data_80051954_slot28[];
extern ObjectRef data_80051958_slot28;
extern u8 data_80032a64_slot28[];
extern u8 data_80032ec8_slot28[];
extern SequenceStep *data_80034518_slot28[];
extern SequenceStep *data_80034520_slot28[];
extern SequenceStep *data_80034530_slot28[];
void func_80019190_slot28(void);
void func_80019130_slot28(Object *obj);
void func_80019100_slot28(Object *obj, int arg);

void func_800189d4_slot28(Object *obj) {
    Object *b;
    Object *prev;
    if (obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x12c;
        data_8018f5a0->pass_index += 1;
        func_80019190_slot28();
        func_80130768(data_8005197c_slot28.p, 0, data_80034518_slot28);
        b = data_80051980_slot28[0];
        func_80019130_slot28(b);
        b->pos_x = 0x60;
        b->pos_y = 0x20;
        b->spawn_x = 0x10;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 4;
        func_80130768(b, 0, data_80034520_slot28);
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            func_80019130_slot28(b);
            data_80051994_slot28.p = b;
            b->pos_x = -0xa0;
            b->pos_y = 0x20;
            b->spawn_x = 0x10;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 4;
            func_80130768(b, 1, data_80034520_slot28);
        }
        b = data_80051984_slot28.p;
        b->field_01 = 0;
        prev = b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x20;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_80032a64_slot28;
            b->data_ptr_b = data_80032ec8_slot28;
            b->pos_x = 0xf8;
            b->pos_y = 0xa0;
            b->countdown = 0x100;
            b->table_index = 4;
            b->field_01 = 1;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 2;
            func_80130768(b, 5, data_80034530_slot28);
            data_80051954_slot28[0] = b;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x20;
            b->table_index = 5;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_80032a64_slot28;
            b->data_ptr_b = data_80032ec8_slot28;
            b->pos_x = 0x150;
            b->pos_y = 0xa0;
            b->countdown = 0x100;
            b->field_01 = 1;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 3;
            b->variant_index = 0;
            func_80130768(b, 6, data_80034530_slot28);
            data_80051958_slot28.p = b;
            b->parent = prev;
            b->box_tables = (BoxTables *)data_80034530_slot28;
        }
        func_80128370();
        func_80019100_slot28(obj, 3);
    }
}
