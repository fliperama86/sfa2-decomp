/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051998_slot28[];
extern ObjectRef data_800519c0_slot28;
extern Object *data_800519c4_slot28[];
extern SequenceStep *data_80036694_slot28[];
extern SequenceStep *data_800366a8_slot28[];
void set_sequence_of_fourth_hud_object_6_slot28(Object *obj, int arg);
void init_object_data_pointers_a_6_slot28(Object *obj);
void release_active_objects_in_table_4_slot28(void);

void advance_pass_when_countdown_hits_zero_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t == 0) {
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}

void advance_pass_and_spawn_sequence_4_object_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        HudState *h = data_8018f5a0;
        h->countdown_timer = 0x1e0;
        h->pass_index = h->pass_index + 1;
        release_active_objects_in_table_4_slot28();
        set_sequence_from_table_5(data_800519c0_slot28.p, 0, data_80036694_slot28);
        p = data_800519c4_slot28[0];
        p->pos_y = -0x50;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_6_slot28(p);
            p->pos_x = 0xd0;
            p->pos_y = 0x70;
            p->countdown = 0x40;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 3;
            set_sequence_from_table_5(p, 4, data_800366a8_slot28);
            data_80051998_slot28[0] = p;
        }
        play_sound_at(0, 0, 0x302);
        set_sequence_of_fourth_hud_object_6_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}
