/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051c64_slot28[];
extern ObjectRef data_80051c8c_slot28;
extern Object *data_80051c90_slot28[];
extern ObjectRef data_80051c94_slot28;
extern SequenceStep *data_8004dcd8_slot28[];
extern SequenceStep *data_8004dce8_slot28[];
void release_active_objects_in_table_15_slot28(void);
void set_sequence_of_fourth_hud_object_17_slot28(Object *obj, int arg);
void init_object_data_pointers_a_17_slot28(Object *obj);

void advance_pass_spawn_slot28_object_g_slot28(Object *obj) {
    HudState *h;
    Object *b;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->countdown_timer = 0x258;
        h->pass_index++;
        release_active_objects_in_table_15_slot28();
        set_sequence_from_table_5(data_80051c8c_slot28.p, 1, data_8004dcd8_slot28);
        b = data_80051c90_slot28[0];
        b->pos_x = 0x48;
        b->pos_y = -0x50;
        b->field_01 = 1;
        b = data_80051c94_slot28.p;
        b->field_01 = 0;
        start_fade_mode_1a01_and_wait();
        set_sequence_of_fourth_hud_object_17_slot28(obj, 3);
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            init_object_data_pointers_a_17_slot28(b);
            b->pos_x = 0xb8;
            b->pos_y = 0xa0;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 2;
            set_sequence_from_table_5(b, 5, data_8004dce8_slot28);
            data_80051c64_slot28[0] = b;
        }
    }
}
