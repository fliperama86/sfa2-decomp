/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051a98_slot28[];
extern ObjectRef data_80051ac0_slot28;
extern Object *data_80051ac4_slot28[];
extern SequenceStep *data_8003f084_slot28[];
extern SequenceStep *data_8003f090_slot28[];
extern SequenceStep *data_8003f09c_slot28[];
void release_active_objects_in_table_8_slot28(void);
void init_object_data_pointers_a_10_slot28(Object *obj);
void set_sequence_of_fourth_hud_object_10_slot28(Object *obj, int arg);

void advance_slot28_pass_if_flag_to_step_two_slot28(Object *obj) {
    Object *b;
    if (menu_active_flag != 0 && obj->field_f0 == 0) {
        screen_state->countdown_timer = 0x258;
        screen_state->pass_index++;
        release_active_objects_in_table_8_slot28();
        set_sequence_from_table_5(data_80051ac0_slot28.p, 2, data_8003f084_slot28);
        b = data_80051ac4_slot28[0];
        b->pos_x = 0x78;
        b->pos_y = 0x20;
        b->field_01 = 1;
        set_sequence_from_table_5(b, 1, data_8003f090_slot28);
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            init_object_data_pointers_a_10_slot28(b);
            b->pos_x = 0xb0;
            b->pos_y = 0xa0;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 3;
            set_sequence_from_table_5(b, 6, data_8003f09c_slot28);
            data_80051a98_slot28[0] = b;
        }
        set_sequence_of_fourth_hud_object_10_slot28(obj, 2);
        start_fade_mode_1a01_and_wait();
    }
}
