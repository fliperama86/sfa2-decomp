/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_8005195c_slot28;
extern ObjectRef data_80051960_slot28;
extern u8 data_80032a64_slot28[];
extern u8 data_80032ec8_slot28[];
extern SequenceStep *data_80034530_slot28[];
void func_80019100_slot28(Object *obj, int arg);

void func_80018458_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *b;
    h->countdown_timer = 0x12c;
    h->pass_index++;
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->active_count = 1;
        b->field_02 = 0x20;
        b->field_90 = (void *)0x80060000;
        b->data_ptr_a = data_80032a64_slot28;
        b->data_ptr_b = data_80032ec8_slot28;
        b->pos_x = 0x48;
        b->pos_y = 0xa0;
        b->spawn_x = 0x60;
        b->spawn_y = 0x1e0;
        b->table_index = 0;
        b->field_01 = 1;
        b->image_row = 0;
        b->ot_depth = 3;
        func_80130768(b, 2, data_80034530_slot28);
        data_8005195c_slot28.p = b;
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->active_count = 1;
        b->field_02 = 0x20;
        b->field_90 = (void *)0x80060000;
        b->data_ptr_a = data_80032a64_slot28;
        b->data_ptr_b = data_80032ec8_slot28;
        b->pos_x = 0x138;
        b->pos_y = 0xa0;
        b->spawn_x = 0x60;
        b->spawn_y = 0x1e0;
        b->table_index = 1;
        b->field_01 = 1;
        b->image_row = 0;
        b->ot_depth = 3;
        func_80130768(b, 3, data_80034530_slot28);
        data_80051960_slot28.p = b;
    }
    func_80128370();
    func_80019100_slot28(obj, 1);
}
