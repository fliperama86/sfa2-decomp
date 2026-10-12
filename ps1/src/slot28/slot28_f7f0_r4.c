/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051ad8_slot28[];
extern Object *data_80051b00_slot28[];
extern SequenceStep *data_80041498_slot28[];
extern SequenceStep *data_800414b0_slot28[];
extern SequenceStep *data_800414b8_slot28[];
void release_active_objects_in_table_9_slot28(void);
void init_object_data_pointers_a_11_slot28(Object *obj);
void set_sequence_of_fourth_hud_object_11_slot28(Object *obj, int arg);

void advance_slot28_pass_if_flag_spawn_two_slot28(Object *obj) {
    Object *b;
    if (*(u8 *)0x80190949 != 0 && obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x258;
        data_8018f5a0->pass_index++;
        release_active_objects_in_table_9_slot28();
        set_sequence_from_table_5(data_80051b00_slot28[0], 0, data_80041498_slot28);
        b = data_80051b00_slot28[1];
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        b->ot_depth = 5;
        b = data_80051b00_slot28[2];
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        set_sequence_from_table_5(b, 1, data_800414b0_slot28);
        b->ot_depth = 4;
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            int two = 2;
            init_object_data_pointers_a_11_slot28(b);
            b->pos_x = 0x98;
            b->pos_y = 0x90;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = two;
            set_sequence_from_table_5(b, 0, data_800414b8_slot28);
            data_80051ad8_slot28[0] = b;
            b->ot_depth = two;
        }
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            init_object_data_pointers_a_11_slot28(b);
            b->pos_x = 0x98;
            b->pos_y = 0x90;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 2;
            set_sequence_from_table_5(b, 1, data_800414b8_slot28);
            data_80051ad8_slot28[1] = b;
            b->ot_depth = 3;
        }
        set_sequence_of_fourth_hud_object_11_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}
