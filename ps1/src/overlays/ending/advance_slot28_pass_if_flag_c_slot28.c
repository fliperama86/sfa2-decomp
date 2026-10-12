/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051ba4_slot28[];
extern Object *data_80051bd0_slot28[];
extern ObjectRef data_80051bdc_slot28;
extern SequenceStep *data_80047bdc_slot28[];
extern SequenceStep *data_80047be4_slot28[];
void release_active_objects_in_table_12_slot28(void);
void init_object_data_pointers_a_14_slot28(Object *obj);
void set_sequence_of_fourth_hud_object_14_slot28(Object *obj, int arg);

void advance_slot28_pass_if_flag_c_slot28(Object *obj) {
    Object *o;
    if (data_80190949 != 0 && obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x258;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        release_active_objects_in_table_12_slot28();
        o = data_80051bd0_slot28[0];
        o->field_01 = 1;
        set_sequence_from_table_5(o, 0, data_80047bdc_slot28);
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            init_object_data_pointers_a_14_slot28(o);
            o->pos_x = 0xe8;
            o->pos_y = 0xa0;
            o->spawn_x = 0x60;
            o->spawn_y = 0x1e0;
            o->image_row = 0;
            o->ot_depth = 2;
            set_sequence_from_table_5(o, 4, data_80047be4_slot28);
            data_80051ba4_slot28[0] = o;
            o->ot_depth = 3;
        }
        start_fade_mode_1a01_and_wait();
        set_sequence_of_fourth_hud_object_14_slot28(obj, 2);
    }
}

void set_countdown_and_next_pass_on_field_negative_slot28(Object *obj) {
    if (((Slot28Obj *)data_80051bdc_slot28.p)->field_3a < 0) {
        data_8018f5a0->countdown_timer = 0xc0;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
    }
}
