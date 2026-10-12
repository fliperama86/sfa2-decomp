/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80031c98_slot28[];
extern Object *data_80051910_slot28[];
void set_sequence_of_fourth_hud_object_4_slot28(Object *obj, int arg);
void init_object_data_pointers_a_4_slot28(Object *obj);
void release_active_objects_in_table_2_slot28(void);

void advance_hud_pass_spawn_three_objects_slot28(Object *obj) {
    Object *o;
    HudState *h;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->countdown_timer = 0x258;
        h->pass_index += 1;
        release_active_objects_in_table_2_slot28();
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            init_object_data_pointers_a_4_slot28(o);
            data_80051910_slot28[11] = o;
            o->pos_x = 0x60;
            o->pos_y = 0x20;
            o->spawn_x = 0x10;
            o->spawn_y = 0x1e0;
            o->image_row = 0;
            o->ot_depth = 4;
            set_sequence_from_table_5(o, 0, data_80031c98_slot28);
        }
        set_sequence_of_fourth_hud_object_4_slot28(obj, 1);
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            o->active_count = 1;
            o->field_02 = 0xa6;
            o->table_index = 3;
            data_80051910_slot28[0] = o;
        }
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            o->active_count = 1;
            o->field_02 = 0xa6;
            o->table_index = 4;
            data_80051910_slot28[1] = o;
        }
        start_fade_mode_1a01_and_wait();
    }
}
