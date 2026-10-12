/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051890_slot28;
extern ObjectRef data_80051894_slot28;
extern Object *data_800518bc_slot28[];
extern SequenceStep *data_8002bddc_slot28[];
extern SequenceStep *data_8002bde4_slot28[];
void init_object_data_pointers_a_2_slot28(Object *obj);

void start_pass_scene_with_two_pooled_objects_slot28(Object *obj) {
    HudState *h;
    Object *p;
    int x;
    int y;
    int one;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        x = 0xf0;
        h->countdown_timer = x;
        h->pass_index = h->pass_index + 1;
        p = data_800518bc_slot28[0];
        set_sequence_from_table_5(p, 1, data_8002bddc_slot28);
        one = 1;
        y = 0x50;
        p->field_01 = one;
        p->pos_x = y;
        p->pos_y = 0x20;
        p->table_index = 0;
        p->ot_depth = 4;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_2_slot28(p);
            p->spawn_x = 0x60;
            p->ot_depth = one;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->pos_x = x;
            p->pos_y = y;
            set_sequence_from_table_5(p, 5, data_8002bde4_slot28);
            data_80051890_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_2_slot28(p);
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->pos_x = 0x78;
            p->ot_depth = one;
            p->image_row = 0;
            p->pos_y = 0xa0;
            set_sequence_from_table_5(p, 0x10, data_8002bde4_slot28);
            data_80051894_slot28.p = p;
        }
        start_fade_mode_1a01_and_wait();
    }
}
