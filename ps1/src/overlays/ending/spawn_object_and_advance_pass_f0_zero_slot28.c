/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_8003b504_slot28[];
extern SequenceStep *data_8003b50c_slot28[];
extern ObjectRef data_80051a40_slot28;
extern Object *data_80051a44_slot28[];
void set_sequence_of_fourth_hud_object_8_slot28(Object *obj, int arg);
void init_object_data_pointers_a_8_slot28(Object *obj);

void spawn_object_and_advance_pass_f0_zero_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        data_8018f5a0->pass_index += 1;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_8_slot28(p);
            data_80051a44_slot28[0] = p;
            p->pos_x = 0x48;
            p->pos_y = -0x60;
            p->spawn_x = 0x10;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 5;
            set_sequence_from_table_5(p, 0, data_8003b50c_slot28);
        }
        p = data_80051a44_slot28[0];
        p->pos_y = 0x20;
        p->ot_depth = 5;
        set_sequence_from_table_5(p, 0, data_8003b50c_slot28);
        set_sequence_from_table_5(data_80051a40_slot28.p, 1, data_8003b504_slot28);
        set_sequence_of_fourth_hud_object_8_slot28(obj, 1);
        start_fade_mode_c01_and_wait();
    }
}
