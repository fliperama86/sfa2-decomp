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

void func_80016658_slot28(Object *obj) {
    Slot28Obj *so = (Slot28Obj *)obj;
    if (obj->field_f0 == 0) {
        HudState *h = data_8018f5a0;
        Object *p;
        int one;
        h->countdown_timer = 0x258;
        h->pass_index++;
        func_80016e7c_slot28();
        p = data_800518fc_slot28[0];
        p->pos_x = 0x80;
        p->pos_y = -0x60;
        func_80130768(data_800518f8_slot28.p, 3, data_8002fc90_slot28);
        p = (Object *)func_8011f1e0();
        one = 1;
        if (p != 0) {
            p->field_02 = 0x72;
            p->table_index = 5;
            p->ot_depth = 4;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->pos_x = 0xc0;
            p->pos_y = 0xa0;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_8002e1d4_slot28;
            p->active_count = one;
            p->field_01 = one;
            p->image_row = 0;
            p->data_ptr_b = data_8002e758_slot28;
            if ((so->state_index & 0x7f) == 4) {
                func_80130768(p, 2, data_8002fca8_slot28);
            } else {
                func_80130768(p, 8, data_8002fca8_slot28);
            }
            data_800518d0_slot28[0] = p;
        }
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xa6;
            p->table_index = 2;
            data_800518d0_slot28[1] = p;
        }
        func_80016e28_slot28(obj, 1);
        func_8012818c();
    }
}
