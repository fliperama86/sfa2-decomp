/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800518d0_slot28[];
extern ObjectRef data_800518f8_slot28;
extern Object *data_800518fc_slot28[];
extern SequenceStep *data_8002fc90_slot28[];
extern SequenceStep *data_8002fca8_slot28[];
extern u8 data_8002e1d4_slot28[];
extern u8 data_8002e758_slot28[];
void func_80016e7c_slot28(void);
void func_80016e28_slot28(Object *obj, int arg);

void func_80016abc_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        HudState *h = data_8018f5a0;
        Object *p;
        int x;
        int one;
        h->countdown_timer = 0x12c;
        h->pass_index++;
        func_80016e7c_slot28();
        x = 0x60;
        p = data_800518fc_slot28[0];
        p->pos_x = x;
        p->pos_y = -0x60;
        func_80130768(data_800518f8_slot28.p, 0, data_8002fc90_slot28);
        p = (Object *)func_8011f1e0();
        one = 1;
        if (p != 0) {
            p->field_02 = 0x72;
            p->table_index = 2;
            p->ot_depth = 4;
            p->spawn_y = 0x1e0;
            p->pos_x = 0xf8;
            p->pos_y = 0x90;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_8002e1d4_slot28;
            p->active_count = one;
            p->field_01 = one;
            p->spawn_x = x;
            p->image_row = 0;
            p->data_ptr_b = data_8002e758_slot28;
            if ((((Slot28Obj *)obj)->state_index & 0x7f) == 4) {
                func_80130768(p, 4, data_8002fca8_slot28);
            } else {
                func_80130768(p, 0xa, data_8002fca8_slot28);
            }
            data_800518d0_slot28[0] = p;
        }
        p = (Object *)func_8011f1e0();
        one = 1;
        if (p != 0) {
            p->field_02 = 0x72;
            p->table_index = 3;
            p->ot_depth = 4;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->pos_x = 0xa0;
            p->pos_y = 0x90;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_8002e1d4_slot28;
            p->active_count = one;
            p->field_01 = one;
            p->image_row = 0;
            p->data_ptr_b = data_8002e758_slot28;
            if ((((Slot28Obj *)obj)->state_index & 0x7f) == 4) {
                func_80130768(p, 6, data_8002fca8_slot28);
            } else {
                func_80130768(p, 0xb, data_8002fca8_slot28);
            }
            data_800518d0_slot28[1] = p;
        }
        func_80016e28_slot28(obj, 4);
        func_80128370();
    }
}
