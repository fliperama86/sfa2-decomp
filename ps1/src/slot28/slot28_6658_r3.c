/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800518d0_slot28[];
extern ObjectRef data_800518f8_slot28;
extern Object *data_800518fc_slot28[];
extern SequenceStep *data_8002fc90_slot28[];
extern SequenceStep *data_8002fca0_slot28[];
extern SequenceStep *data_8002fca8_slot28[];
extern u8 data_8002e1d4_slot28[];
extern u8 data_8002e758_slot28[];
void release_active_objects_in_table_slot28(void);
void set_sequence_of_fourth_hud_object_3_slot28(Object *obj, int arg);

void start_pass_three_spawn_one_object_slot28(Object *obj) {
    Slot28Obj *so = (Slot28Obj *)obj;
    if (obj->field_f0 == 0) {
        HudState *h = data_8018f5a0;
        Object *p;
        int x;
        int one;
        h->countdown_timer = 0x258;
        h->pass_index++;
        release_active_objects_in_table_slot28();
        set_sequence_from_table_5(data_800518f8_slot28.p, 1, data_8002fc90_slot28);
        x = 0x60;
        p = data_800518fc_slot28[0];
        one = 1;
        p->pos_x = x;
        p->pos_y = 0x20;
        p->field_01 = one;
        set_sequence_from_table_5(p, 1, data_8002fca0_slot28);
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->field_02 = 0x72;
            p->table_index = 6;
            p->ot_depth = 4;
            p->spawn_y = 0x1e0;
            p->pos_x = 0xb0;
            p->pos_y = 0xa0;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_8002e1d4_slot28;
            p->active_count = one;
            p->field_01 = one;
            p->spawn_x = x;
            p->image_row = 0;
            p->data_ptr_b = data_8002e758_slot28;
            if ((so->state_index & 0x7f) == 4) {
                set_sequence_from_table_5(p, 3, data_8002fca8_slot28);
            } else {
                set_sequence_from_table_5(p, 9, data_8002fca8_slot28);
            }
            data_800518d0_slot28[0] = p;
        }
        set_sequence_of_fourth_hud_object_3_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}
